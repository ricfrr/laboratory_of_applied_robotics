/*!****************************************************************************
 *    \file    kalman_filter.hpp
 *    \brief   Class that implement a basic kalman filter (code taken from Udacity) it is a template on the dimention of the state
 *    \version 1.0
 *    \date    2017
 *****************************************************************************/
/// @file   Algorithm.hpp
/// @brief  Class that implement a basic kalman filter (code taken from Udacity)
/// @author Valerio Magnago
///

#ifndef KALMAN_FILTER_H_
#define KALMAN_FILTER_H_


#include <Eigen/Dense>
#include <Eigen/Core>

#include <iostream>
#include <vector>

typedef Eigen::Matrix<double, 5, 5> Matrix5;
typedef Eigen::Matrix<double, 3, 3> Matrix3;
typedef Eigen::Matrix<double, 2, 2> Matrix2;
typedef Eigen::Matrix<double, 5, 1> Vec5;
typedef Eigen::Matrix<double, 3, 1> Vec3;
typedef Eigen::Matrix<double, 2, 1> Vec2;
typedef Eigen::Matrix<double, 5, 2> Matrix52;
typedef Eigen::Matrix<double, 2, 5> Matrix25;
typedef Eigen::Matrix<double, 5, 3> Matrix53;
typedef Eigen::Matrix<double, 3, 5> Matrix35;


class ExtendedKalmanFilter {
public:
  bool initialized;

  typedef struct PastState
  {
    Vec5 x;
    Matrix5 P;
    double timestamp;

	PastState(const Vec5& x_, 
		      const Matrix5& P, 
		      const double& timestamp):
	    x(x_), P(P), timestamp(timestamp)
	{}
  } PastState;

  //std::vector<PastState> states;

  Vec5 x;
  Matrix5 P;
  double timestamp;

	/**
	 * Constructor
	 */
	ExtendedKalmanFilter();

	/**
	 * Prediction Predicts the state and the state covariance
	 * using the process model. x_ = F_ * x_ +w_k
	 * @param x_ is the state of the system, that will be update
	 * @param P_ is the covariance of the state that will be update
	 * @param F is the state transition matrix (keeped constant)
	 * @param Q is the process covariance matrix  (keeped constant)
	 */
	static void Predict(Vec5& x_, Matrix5& P_, double dt); // TODO: private

  /**
	 * Prediction Predicts the state and the state covariance
	 * using the process model. x_ = F_ * x_ +w_k
	 * @param x_ is the state of the system, that will be update
	 * @param P_ is the covariance of the state that will be update
	 * @param F is the state transition matrix (keeped constant)
	 * @param Q is the process covariance matrix  (keeped constant)
	 */
	void Predict(double timestamp);

	/**
	 * Updates the state by using standard linear Kalman Filter update equations
	 * @param x_ is the state of the system, that will be update
	 * @param P_ is the covariance of the state that will be update
	 * @param z The measurement at k+1
	 * @param H Is the measurement matrix
	 * @param R is the measurement covariance matrix
	 */
  void UpdateCamera(const Vec3& z, const Matrix3& R, double timestamp);

  /**
	 * Updates the state by using standard linear Kalman Filter update equations
	 * @param x_ is the state of the system, that will be update
	 * @param P_ is the covariance of the state that will be update
	 * @param z The measurement at k+1
	 * @param H Is the measurement matrix
	 * @param R is the measurement covariance matrix
	 */
  void UpdateOdometry(const Vec2& z, const Matrix2& R, double timestamp);

	/**
	 * Updates the state by using standard linear Kalman Filter update equations
	 * @param x_ is the state of the system, that will be update
	 * @param P_ is the covariance of the state that will be update
	 * @param z The measurement at k+1
	 * @param H Is the measurement matrix
	 * @param R is the measurement covariance matrix
	 */
  //void Update(const Vec2& z, const Matrix2& R, double timestamp);

	bool isLocalized();

};


#endif /* KALMAN_FILTER_H_ */
