#include "robot_project.h"
#include <atomic>
#include <unistd.h>

// cv::Mat result, bg_img;
// int idx = 0;
// std::string name;
// std::atomic<bool> done;
// int n;

// static void mouseCallback(int event, int x, int y, int, void* p)
// {
// 	if (event != cv::EVENT_LBUTTONDOWN || done.load()) return;

// 	if (result.rows <= idx) throw std::runtime_error("SOmething went wrong");
// 	result.at<float>((int)idx, 0) = x;
// 	result.at<float>((int)idx++, 1) = y;
// 	cv::circle(bg_img, {x,y}, 20, cv::Scalar(0,0,255), -1);
// 	cv::imshow(name.c_str(), bg_img);

// 	if (idx >= n) {
// 		usleep(500*1000);
// 		done.store(true);
// 	}
// }

// static cv::Mat pickNPoints(int n0, cv::Mat const & img)
// {
// 	result = cv::Mat(n0, 2, CV_32F);
// 	idx = 0;
// 	bg_img = img.clone();
// 	name = "Pick " + std::to_string(n0) + " points";
// 	cv::imshow(name.c_str(), bg_img);
// 	cv::namedWindow(name.c_str());
// 	n = n0;

// 	done.store(false);

// 	cv::setMouseCallback(name.c_str(), &mouseCallback, nullptr);
// 	while (!done.load()) {
// 		cv::waitKey(500);
// 	}

// 	cv::destroyWindow(name.c_str());
// 	return result;
// }

// bool RobotProject::preprocessMap(cv::Mat const & img)
// {
// 	cv::Mat corner_pixels = pickNPoints(4, img);

// 	//cv::Mat screen_pixels = cv::Mat(img.rows, img.cols, CV_8UC3, cv::Scalar(0,0,0));
// 	//cv::Mat dst_pixels = pickNPoints(2, screen_pixels);

// 	float origin_x = 0,
// 		    origin_y = 0;

// 	float delta_x = 1920;
// 	float delta_y = 1080;

// 	float delta_x_m = 1.500;
// 	float delta_y_m = 1.000;

// 	float scale_x = delta_x/delta_x_m;
// 	float scale_y = delta_y/delta_y_m;
// 	float scaling_px_m = std::min(scale_x, scale_y);
//  scaling_m_px = 1./scaling_px_m;
// 	delta_x = scaling_px_m*delta_x_m;
// 	delta_y = scaling_px_m*delta_y_m;

//   float offset_x = (1920-delta_x)*0.5;
//   float offset_y = (1080-delta_y)*0.5;

//   origin_x += offset_x;
//   origin_y -= offset_y;
//   origin_x_m = origin_x*scaling_m_px;
//   origin_y_m = origin_y*scaling_m_px;

//   std::cout << "origin_x_m = " << origin_x_m << ";" << std::endl;
//   std::cout << "origin_y_m = " << origin_y_m << ";" << std::endl;
//   std::cout << "scaling_m_px = " << scaling_m_px << ";" << std::endl;

//     cv::Mat transf_pixels = (cv::Mat_<float>(4,2) << origin_x, origin_y,
// 				  			 			   		                       origin_x+delta_x, origin_y,
// 										   		                         origin_x+delta_x, origin_y+delta_y,
// 										   		                         origin_x, origin_y+delta_y);

// 	extrinsic_transform = cv::getPerspectiveTransform(corner_pixels, transf_pixels);
//   std::cout << extrinsic_transform << std::endl;
// 	cv::Mat warpedFrame;
//   cv::warpPerspective(img, warpedFrame, extrinsic_transform, img.size());
//   cv::imshow("PREPROCESS", warpedFrame);
//   cv::waitKey(0);
// 	return false;
// }

// bool RobotProject::preprocessMap(cv::Mat const & img)
// {
// 	cv::Mat corner_pixels = cv::Mat(4, 2, CV_32F);
//   corner_pixels.at<float>(0, 0) = 408;
//   corner_pixels.at<float>(0, 1) = 1080-169;
//   corner_pixels.at<float>(1, 0) = 1513;
//   corner_pixels.at<float>(1, 1) = 1080-214;
//   corner_pixels.at<float>(2, 0) = 1476;
//   corner_pixels.at<float>(2, 1) = 1080-934;
//   corner_pixels.at<float>(3, 0) = 380;
//   corner_pixels.at<float>(3, 1) = 1080-894;
//
// 	//cv::Mat screen_pixels = cv::Mat(img.rows, img.cols, CV_8UC3, cv::Scalar(0,0,0));
// 	//cv::Mat dst_pixels = pickNPoints(2, screen_pixels);
//
// 	float origin_x = 0,
// 		  origin_y = 0;
//
// 	float delta_x = 1920;
// 	float delta_y = 1080;
//
// 	float delta_x_m = 1.500;
// 	float delta_y_m = 1.000;
//
// 	float scale_x = delta_x/delta_x_m;
// 	float scale_y = delta_y/delta_y_m;
// 	float scaling_px_m = std::min(scale_x, scale_y);
//    scaling_m_px = 1./scaling_px_m;
// 	delta_x = scaling_px_m*delta_x_m;
// 	delta_y = scaling_px_m*delta_y_m;
//
//    float offset_x = (1920-delta_x)*0.5;
//    float offset_y = (1080-delta_y)*0.5;
//
//    origin_x += offset_x;
//    origin_y -= offset_y;
//    origin_x_m = origin_x*scaling_m_px;
//    origin_y_m = origin_y*scaling_m_px;
//
//    std::cout << "origin_x_m = " << origin_x_m << ";" << std::endl;
//    std::cout << "origin_y_m = " << origin_y_m << ";" << std::endl;
//    std::cout << "scaling_m_px = " << scaling_m_px << ";" << std::endl;
//
//    cv::Mat transf_pixels = (cv::Mat_<float>(4,2) << origin_x, origin_y,
// 				  			 			   		                       origin_x+delta_x, origin_y,
// 										   		                         origin_x+delta_x, origin_y+delta_y,
// 										   		                         origin_x, origin_y+delta_y);
//
// 	extrinsic_transform = cv::getPerspectiveTransform(corner_pixels, transf_pixels);
//   std::cout << extrinsic_transform << std::endl;
// 	cv::Mat warpedFrame;
//   cv::warpPerspective(img, warpedFrame, extrinsic_transform, img.size());
//   cv::imshow("PREPROCESS", warpedFrame);
//   cv::waitKey(0);
// 	return false;
// }


//bool RobotProject::preprocessMap(cv::Mat const & img)
//{
//  extrinsic_transform = (cv::Mat_<float>(3, 3) << 1.630957553623722, -0.04745129143591548, -556.973176374374,
//                                                  -0.06339225781026706, -1.625442507955552, 1464.426173117475,
//                                                  -6.928724316948662e-07, 1.068792867053213e-05, 1);
//  origin_x_m = 0.138889;
//  origin_y_m = 0;
//
//  scaling_m_px = 1./1080.;
//  return true;
//}


// OFFICIAL
bool RobotProject::preprocessMap(cv::Mat const & img)
{
  extrinsic_transform = (cv::Mat_<float>(3, 3) << -1.282196857452037, 0.02380961966496503, 2216.442282351578,
 												   0.005792655163075588, 1.220319354354702, -122.8390453881923,
 												  -4.209846151677398e-06, 1.505754882817156e-05, 1);
  origin_x_m = 0.138889;
  origin_y_m = 0;

  //scaling_m_px = 1./1080.;
    scaling_m_px = 0.000925926;
  return true;
}

bool RobotProject::localize(cv::Mat const & img, 
                            std::vector<double> & state)
{
  cv::Mat warpedFrame, frameThreshold;
  cv::warpPerspective(img, warpedFrame, extrinsic_transform, img.size());

  cv::cvtColor(warpedFrame, frameThreshold, cv::COLOR_BGR2HSV);
  cv::inRange(frameThreshold, cv::Scalar(80, 120, 150), cv::Scalar(120, 220, 230), frameThreshold);

  // Process blue mask
  std::vector<std::vector<cv::Point>> contours, contours_approx;
  std::vector<cv::Point> approx_curve;
  cv::findContours(frameThreshold, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  //drawContours(warpedFrame, contours, -1, cv::Scalar(40,190,40), 1, cv::LINE_AA);
  std::cout << "N. contours: " << contours.size() << std::endl;

  bool found = false;
  for (int i=0; i<contours.size(); ++i)
  {
    //std::cout << (i+1) << ") Contour size: " << contours[i].size() << std::endl;
    cv::approxPolyDP(contours[i], approx_curve, 30, true);
    if (approx_curve.size() != 3) continue;
    double area = cv::contourArea(approx_curve);
    if (area<4000 || area>7000) continue;
    //std::cout << "Area: " << area << std::endl;
    contours_approx = {approx_curve};
    cv::drawContours(warpedFrame, contours_approx, -1, cv::Scalar(0,170,220), 3, cv::LINE_AA);
    found = true;
    break;
  }

  if (found) 
  {
    double cx, cy;
    for (auto item: approx_curve) 
    {
      cx += item.x;
      cy += item.y;
    }
    cx /= approx_curve.size();
    cy /= approx_curve.size();

    cv::Point center(cx, cy);

    double dst = 0;
    cv::Point vertex;
    for (cv::Point item: approx_curve)
    {
      double dx = item.x-cx;      
      double dy = item.y-cy;
      double curr_d = dx*dx + dy*dy;
      if (curr_d > dst)
      { 
        dst = curr_d;
        vertex = item;
      }
    }
    
    // cv::Moments m = cv::moments(approx_curve, false);
    // cv::Point center(m.m10/m.m00, m.m01/m.m00);
    // cv::Vec4f line;
    // cv::fitLine(approx_curve, line, cv::DIST_L2, 0, 0.01, 0.01);
    // cv::line(warpedFrame, cv::Point(line[2], line[3]), cv::Point(line[2]+line(0)*80, line(3)+line(1)*80), (0,255,0), 2);


   // cv::line(warpedFrame, center, vertex, (0,255,0), 2);
   // cv::circle(warpedFrame, center, 20, cv::Scalar(0,0,0), -1);

    double dx = center.x-vertex.x;
    double dy = center.y-vertex.y;
    double theta = std::atan2(dy, dx);

    double xc_m = cx*scaling_m_px - origin_x_m;
    double yc_m = cy*scaling_m_px - origin_y_m;

    state = { xc_m, yc_m, theta };
    //covariance = {};

    std::cout << xc_m << " " << yc_m << " " << theta*180/M_PI << std::endl;
  }

 // cv::cvtColor(frameThreshold, frameThreshold, CV_GRAY2BGR);
 // cv::addWeighted(warpedFrame, 0.5, frameThreshold, 0.5, 0.0, warpedFrame);
 // cv::resize(warpedFrame, warpedFrame, cv::Size(warpedFrame.cols/2, warpedFrame.rows/2)); //resize image
 // cv::imshow("LOC", warpedFrame);
 // cv::waitKey(1);

  return found;

}

static inline
double sinc(double t)
{
  double s;
  if (std::abs(t)<0.002) 
  { 
    s = 1. - t*t/6.*(1. - t*t/20.);
  }
  else
  {
    s = std::sin(t)/t;
  }
  return s;
}

bool RobotProject::planPath(cv::Mat const & img, Path & path) 
{
  std::vector<Pose> points;

  //   double x0 = 0.;
  //   double y0 = 0.5;
  //   double th = 0;
  //   double kappa = -10;
  //   double s0 = 0;

  // for (double s=0; s<=1.0; s+=0.02) {
  //   double x = x0 + s;
  //   double y = y0;            
  //   points.emplace_back(s+s0, x, y, th, 0);
  // }

  std::vector<double> xs, ys, ths, ks, ls;
  xs = {0.2, 0.2696, 0.8304, 0.9, 0.98373, 1.3163};
  ys = {0.2, 0.2282, 0.7718, 0.8, 0.75467, 0.24533};
  ths = {0, 0.76983, 0.76983, 0, 5.2908, 5.2908};
  ks = {10, 0, -10, -10, 0, 10};
  ls = {0.076983, 0.78102, 0.076983, 0.09924, 0.60828, 0.09924};

  points.emplace_back(0, xs[0], ys[0], ths[0], ks[0]);
  
  int n = xs.size();
  for (int i=0; i<n; ++i) 
  {
    double x0 = xs[i];   
    double y0 = ys[i];    
    double th = ths[i];    
    double kappa = ks[i];    
    double L = ls[i];
    double s0 = points.back().s;
    for (double s=0.01; s<=L; s+=0.01) 
    {
      //double x = x0 + (std::sin(kappa*s+th)-std::sin(th))*1./kappa;
      double x = x0 + (s*sinc(0.5*kappa*s)*std::cos(th+0.5*kappa*s));
      //double y = y0 - (std::cos(kappa*s+th)-std::cos(th))*1./kappa;            
      double y = y0 + (s*sinc(0.5*kappa*s)*std::sin(th+0.5*kappa*s));
      points.emplace_back(s+s0, x, y, kappa*s+th, kappa);
    }
  }
 

  // x0 = points.back().x;
  // y0 = points.back().y;
  // th = points.back().theta;
  // s0 = points.back().s;
  // kappa = -kappa;
  // for (double s=0.02; s<=1; s+=0.02) {
  //   double x = x0 + (std::sin(kappa*s+th)-std::sin(th))*1./kappa;
  //   double y = y0 - (std::cos(kappa*s+th)-std::cos(th))*1./kappa;            
  //   points.emplace_back(s+s0, x, y, kappa*s+th, kappa);
  // }

    path.setPoints(points);
    return true;
}
