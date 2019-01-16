#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <limits>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <mutex>

#include "camera_capture.h"
#include "extended_kalman_filter.h"
#include "path_following.h"
#include "robot_project.h"
#include "utils.h"

extern "C"
{
  #include "libs/client_bt.h"
  #include "libs/client_utils.h"
  #include "common/pkt_serialization.h"
}

#define LOG

std::mutex guard;
ExtendedKalmanFilter ekf;
PathFollowing pf;

struct cmd_pkt cmd = { 0, 0 };
int brick_socket = 0;

static volatile int terminating = 0;

static const int CONTROL_PERIOD_MS = 20;
static const int CAMERA_PERIOD_MS  = 50;

double PIXEL_SCALE=1;
double ROBOT_PIXEL_SCALE=1;


void intHandler(int dummy)
{
  terminating = 1;
}





void* send_thread_func(void* arg)
{
  uint8_t buffer[PKT_BUFFER_SIZE];
  size_t size;

#ifdef LOG
  std::ofstream file("data/control.txt");
  const Eigen::IOFormat fmt(10, 0, " ", " ", "", "", "", "");
#endif

  while (!terminating)
  {

    if (pf.endOfPath() || !ekf.isLocalized())
    {
      cmd.v = 0.;
      cmd.omega = 0.;
    }
    else
    {
      double timestamp;
      Vec5 x;
      Matrix5 P;
      {
        std::unique_lock <std::mutex> lock(guard);

        x = ekf.x;
        P = ekf.P;
        timestamp = ekf.timestamp;
      }

      double curr_timestamp =timeutils::getTimeS();
      if(std::abs(curr_timestamp-timestamp)>0.2){
        std::cout<<"tempo elevato:" << curr_timestamp-timestamp << std::endl;
      }

      ExtendedKalmanFilter::Predict(x, P, curr_timestamp - timestamp);

#ifdef LOG
      file << curr_timestamp << " " << x.format(fmt) << "\t" << ekf.x.format(fmt) << std::endl;
      file.flush();
#endif

      double rho = pf.computeControl(x(0), x(1), x(2));
      cmd.v = 0.1;
      cmd.omega = rho * cmd.v;
    }
    fill_cmd_msg(&cmd, buffer, &size);
    if (brick_bt_send(brick_socket, buffer, size) < 0)
    {
      fprintf(stderr, "Failed to send data\n");
    }
    usleep(1000*CONTROL_PERIOD_MS);
  }
  return 0;
}





void* recv_thread_func(void* arg)
{
  uint8_t buffer[PKT_BUFFER_SIZE];
  struct status_pkt status;
  int res;

  double delta_t = std::numeric_limits<double>::max();
  int   read    = 0;

#ifdef LOG
  std::ofstream file("data/odometry.txt");
  const Eigen::IOFormat fmt(10, 0, " ", " ", "", "", "", "");
#endif

  while (!terminating)
  {
    if ((res = brick_bt_recv(brick_socket, buffer+read, STATUS_PKT_SIZE-read, 100)) < 0)
    {
      if (res == -2)
      {
        // Timeout
      }
      else
      {
        fprintf(stderr, "Failed to receive data\n");
      }
    }
    else
    {
      read += res;
      if (read == STATUS_PKT_SIZE)
      {
        read = 0;
        parse_status_msg(buffer+PKT_HEADER_SIZE, &status);


        double curr_time = timeutils::getTimeS();
        double robot_time = status.time*1e-3;
        double tmp = curr_time - robot_time;
        if (tmp<delta_t) 
        {
          delta_t = tmp;
        }

        Vec2 u;
        u << status.v, status.omega;
        Matrix2 R;
        R << status.R11, status.R12, status.R12, status.R22;

#ifdef LOG
        file << curr_time << " " << robot_time+delta_t << " " << delta_t << " " << u.format(fmt) << " " << ekf.x.format(fmt) << " ";
#endif

        {
          std::unique_lock<std::mutex> lock(guard);
          ekf.UpdateOdometry(u, R, robot_time + delta_t );
        }

#ifdef LOG
        file << ekf.x.format(fmt) << std::endl;
        file.flush();
#endif
      }
    }
  }

  return 0;
}






int main(int argc, char * argv[])
{
  signal(SIGINT, intHandler);

  // parse mac address
  client_options cmd_options;
  if (opts_parse_config(&cmd_options, argc, argv) == -1)
  {
    return -1;
  }

  CameraCapture::input_options_t options(1080, 1920, 30, 0);
  CameraCapture camera(options);
  
  std::string filename = "config/intrinsic_calibration_levico.xml";
  if (!camera.loadCoefficients(filename))
  {
    std::cerr << "(Critical) Failed to load configuration" << std::endl;
    return -1;
  }


  cv::Mat img;
  double frame_time;
  if (!camera.grab(img, frame_time))
  {
    std::cerr << "(Critical) Failed to grab frame" << std::endl;
    return -1;
  }

  RobotProject rp(argc, argv);
  if (!rp.preprocessMap(img))
  {
    std::cerr << "(Critical) Failed to preprocess map" << std::endl;
    return false;
  }

  Path path;
  if (!rp.planPath(img, path))
  {
    std::cerr << "(Critical) Failed to plan path" << std::endl;
    return false;
  }

#ifdef LOG
  std::ofstream file("data/path.txt");
  for (auto pt: path.points)
  {
    file << pt.s << " " << pt.x << " " << pt.y << " " << pt.theta << " " << pt.kappa << "\n";
  }
  file.close();
#endif

  while (brick_bt_connect_device(&brick_socket, cmd_options.mac) < 0)
  {
    perror("Failed to open connection");
    if (terminating) return 0;
    usleep(1000*1000);
  }

  // run thread comm_thread
  pthread_t send_thread, recv_thread;
  pthread_create(&send_thread, NULL, send_thread_func, NULL);
  pthread_create(&recv_thread, NULL, recv_thread_func, NULL);

  pf.setPath(path);

#ifdef LOG
  std::ofstream file_measure("data/measure.txt");
  const Eigen::IOFormat fmt(10, 0, " ", " ", "", "", "", "");
#endif

  while (!terminating && !pf.endOfPath())
  {
    cv::Mat imgNew;
    if (!camera.grab(imgNew, frame_time))
    {
      std::cerr << "(Warning) Failed to grab frame" << std::endl;
      continue;
    }

    double t0 = timeutils::getTimeS();
    std::vector<double> state;
    std::vector<std::vector<double>> covariance;
    if (!rp.localize(imgNew, state))
    {
      std::cerr << "(Warning) Failed localization" << std::endl;
      continue;
    }
    
    double t1 = timeutils::getTimeS();
    std::cerr << "DT: " << (t1-t0)*1000 << " ms" << std::endl;

    int size = state.size();
    covariance = {{0.002*0.002, 0, 0}, {0, 0.002*0.002, 0}, {0, 0, 0.005*0.005}};

    Vec3 z;
    Matrix3 R;
    for (int i=0; i<size; ++i) 
    {
      z(i) = state[i];
      for (int j=0; j<size; ++j) 
      {
        R(i,j) = covariance[i][j];
      }  
    }

#ifdef LOG    
    file_measure << frame_time << " " << z(0) << " " << z(1) << " " << z(2) << std::endl;
#endif

    {
      std::unique_lock <std::mutex> lock(guard);
      ekf.UpdateCamera(z, R, frame_time);
    }

    usleep(1000*CAMERA_PERIOD_MS); // TODO: use c++ timer
  }
}
