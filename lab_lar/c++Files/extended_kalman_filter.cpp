/*!****************************************************************************
 *    \file    kalman_filter.hpp
 *    \brief   Class that implement a basic kalman filter (code taken from Udacity) it is a template on the dimention of the state
 *    \version 1.0
 *    \date    2017
 *****************************************************************************/
/// @file   Algorithm.hpp
/// @brief  Class that implement a basic kalman filter (code taken from Udacity)
/// @author Valerio Magnago, Paolo Bevilacqua
///

#include "extended_kalman_filter.h"
#include "utils.h"

const double MAX_DELAY = 0.5;
const size_t MAX_WINDOW_SIZE = 100;


static inline double rangeSymm(double ang) 
{
  while (ang <= - M_PI) ang += 2 * M_PI;
  while (ang >  M_PI) ang -= 2 * M_PI;
  return ang;
}


/**
 * Constructor
  */
ExtendedKalmanFilter::ExtendedKalmanFilter(): 
  initialized(false)
{};  // Nothing to inizialize


/**
 * Prediction Predicts the state and the state covariance
 * using the process model. x_ = F_ * x_ +w_k
 * @param x_ is the state of the system, that will be update
 * @param P_ is the covariance of the state that will be update
 * @param F is the state transition matrix (keeped constant)
 * @param Q is the process covariance matrix  (keeped constant)
 */
void ExtendedKalmanFilter::Predict(Vec5& x_, Matrix5& P_, double dt)
{
  // predict next state using Kalman filter.
  double ds  = x_(3)*dt;
  double dth = x_(4)*dt;

  Matrix5 F = Matrix5::Identity();

  double x, y, theta;
  if (std::abs(dth)<1e-2)
  {
    x = x_(0) + ds*std::cos(x_(2) + dth/2);
    y = x_(1) + ds*std::sin(x_(2) + dth/2);   
    theta = x_(2) + dth;

    F(0,2) = -ds*std::sin(x_(2) + dth/2);
    F(0,3) =  dt*std::cos(x_(2)  + dth/2); 
    F(0,4) = -ds*std::sin(x_(2)  + dth/2)*dt/2; 

    F(1,2) = ds*std::cos(x_(2)  + dth/2); 
    F(1,3) = dt*std::sin(x_(2)  + dth/2); 
    F(1,4) = ds*std::cos(x_(2)  + dth/2)*dt/2;         

    F(2,4) = dt;
  }
  else
  {
    double R = ds/dth;
    x = x_(0) + (std::sin(dth+x_(2))-std::sin(x_(2)))*R;
    y = x_(1) - (std::cos(dth+x_(2))-std::cos(x_(2)))*R;            
    theta = x_(2) + dth;

    F(0,2) = R*(std::cos(x_(2)+dth)-std::cos(x_(2)));
    F(0,3) = (std::sin(dth+x_(2))-std::sin(x_(2)))*dt/dth;
    F(0,4) = -(std::sin(dth+x_(2))-std::sin(x_(2)))*ds/(dth*x_(4)) + R*(std::cos(dth+x_(2))*dt);

    F(1,2) = R*(std::sin(x_(2)+dth)-std::sin(x_(2)));
    F(1,3) = -(std::cos(dth+x_(2))-std::cos(x_(2)))*dt/dth;
    F(1,4) = (std::cos(dth+x_(2))-std::cos(x_(2)))*ds/(dth*x_(4)) - R*(std::sin(dth+x_(2))*dt);
  }
 
  
  Matrix5 Qtmp = Matrix5::Zero();
  /*
  Qtmp(0,0) = 0.1;
  Qtmp(1,1) = 0.1;
  Qtmp(2,2) = 0.5;
  */

  Matrix2 Q = Matrix2::Zero();
  Q(0,0) = 0.01;
  Q(1,1) = 1;

  Matrix52 V = Matrix52::Zero();
  V(0,0) = dt*dt/2*std::cos(x_(2));
  V(1,0) = dt*dt/2*std::sin(x_(2));
  V(2,1) = dt*dt/2;
  V(3,0) = dt;
  V(4,1) = dt;

  //V << dt*std::cos(x_(3)), 
  P_ = F*P_*(F.transpose()) + V*Q*V.transpose() + Qtmp; 

  x_(0) = x; x_(1) = y; x_(2) = theta;
}

void ExtendedKalmanFilter::Predict(double _timestamp)
{
  if (!initialized) return;
  double dt = _timestamp - timestamp;
  Predict(x, P, dt);
}


/**
 * Updates the state by using standard linear Kalman Filter update equations
 * @param x_ is the state of the system, that will be update
 * @param P_ is the covariance of the state that will be update
 * @param z The measurement at k+1
 * @param H Is the measurement matrix
 * @param R is the measurement covariance matrix
 */
void ExtendedKalmanFilter::UpdateCamera(const Vec3& z, const Matrix3& R, double _timestamp)
{
  /**
  * update the state by using Kalman Filter equations
  */    

  double dt = _timestamp-timestamp;
  if (!initialized || std::abs(dt)>MAX_DELAY)
  {
    std::cerr << " Reinit the filter " << std::endl;
    x.topRows(3) = z;
    P = Matrix5::Zero();
    P.topLeftCorner(3,3) = R;
    P(3,3) = P(4,4) = 10000; // high value
    initialized = true;   
    timestamp = _timestamp;
    return;
  }

  //std::cout << dt <<std::endl;
  Predict(x, P, dt);

  const Matrix35 H = Matrix35::Identity();
  Vec3 y = z - H * x;
  const Matrix53 Ht = H.transpose();
  const Matrix3 S = H * P * Ht + R;
  const Matrix3 Si = S.inverse();
  const Matrix53 K =  P * Ht * Si;
  const Matrix5 I = Matrix5::Identity();

  y(2) = rangeSymm(y(2));
  x = x + (K * y);  // new state
  P = (I - K * H) * P; // new covariance
  timestamp = _timestamp;
}


/**
 * Updates the state by using standard linear Kalman Filter update equations
 * @param x_ is the state of the system, that will be update
 * @param P_ is the covariance of the state that will be update
 * @param z The measurement at k+1
 * @param H Is the measurement matrix
 * @param R is the measurement covariance matrix
 */
void ExtendedKalmanFilter::UpdateOdometry(const Vec2& z, const Matrix2& R, double _timestamp)
{
  /**
  * update the state by using Kalman Filter equations
  */    

  double dt = _timestamp-timestamp;
  if (!initialized || dt<0)
  {
    return;
  }
  
  Predict(x, P, dt);

  Matrix25 H = Matrix25::Zero();  
  H.topRightCorner(2,2) = Matrix2::Identity();
  Vec2 y = z - H * x;
  const Matrix52 Ht = H.transpose();
  const Matrix2 S = H * P * Ht + R;
  const Matrix2 Si = S.inverse();
  const Matrix52 K =  P * Ht * Si;
  const Matrix5 I = Matrix5::Identity();

  x = x + (K * y);  // new state
  P = (I - K * H) * P; // new covariance
  timestamp = _timestamp;

}


bool ExtendedKalmanFilter::isLocalized()
{
  if (!initialized) {
    return false;
  }
  // double currTime = timeutils::getTimeS();
  // if ((currTime-states.back().timestamp) > 0.5) return false;
  // if (std::max(states.back().P(0,0),states.back().P(1,1)) > 0.04) return false;
  return true;
}
