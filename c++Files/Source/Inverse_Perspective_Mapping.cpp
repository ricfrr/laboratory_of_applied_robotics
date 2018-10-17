//
//  Inverse_Perspective_Mapping.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 05.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Inverse_Perspective_Mapping.hpp"
#include "../Headers/Arena.hpp"

Inverse_Perspective_Mapping::Inverse_Perspective_Mapping()
{
    std::cout << "Inverse Perspective Mapping DONE" << std::endl;
}

Inverse_Perspective_Mapping::~Inverse_Perspective_Mapping()
{
    // nada
}

void Inverse_Perspective_Mapping::loadCoefficients(const std::string &filename,
                                                   cv::Mat &camera_matrix,
                                                   cv::Mat &dist_coeffs)
{
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        throw std::runtime_error("Could not open file " + filename);
    }
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> dist_coeffs;
    fs.release();
}

void computeDistanceandSwap(std::vector<cv::Point> &corner)
{

    double first_side = cv::norm(corner[0] - corner[1]);
    double second_side = cv::norm(corner[1] - corner[2]);
    std::cout << "FIRST side : " << first_side << "  Second side" << second_side << std::endl;

    if (first_side >= second_side)
    {
        cv::Point tmp_corner = corner[0];
        corner[0] = corner[3];
        corner[3] = corner[2];
        corner[2] = corner[1];
        corner[1] = tmp_corner;
    }
}

std::vector<cv::Point> findCorners(Mat img)
{
    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    // Preparing the kernel matrix
    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT, cv::Size((1 * 2) + 1, (1 * 2) + 1));

    // Definining contour containers
    cv::Mat contours_img;
    std::vector<std::vector<cv::Point>> contours, contours_approx;
    std::vector<cv::Point> approx_curve;
    std::vector<cv::Point> arena;

    // Find black regions, modified color range in order to detect the shape
    cv::Mat black_mask;
    cv::inRange(hsv_img, cv::Scalar(0, 0, 0), cv::Scalar(180, 90, 90),
                black_mask); //

    // Filter (applying dilation, blurring, dilation and erosion) the image
    kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                       cv::Size((4 * 2) + 1, (4 * 2) + 1));
    cv::dilate(black_mask, black_mask, kernel);
    cv::GaussianBlur(black_mask, black_mask, cv::Size(9, 9), 6, 6);
    cv::dilate(black_mask, black_mask, kernel);
    cv::erode(black_mask, black_mask, kernel);

    // cv::imshow("BLACK_filter", black_mask);
    // cv::moveWindow("BLACK_filter", W_0+2*(img.cols+OFFSET_W), H_0);

    // Process black mask
    contours_img = img.clone();
    cv::findContours(black_mask, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
                 cv::LINE_AA);
    for (int i = 0; i < contours.size(); ++i)
    {
        approxPolyDP(contours[i], approx_curve, 70, true);
        contours_approx = {approx_curve};
        if (approx_curve.size() == 4)
        {
            //std::cout << "corners :  " << approx_curve << std::endl;
            Scalar color = Scalar(0, 0, 255);
            // swapping corners in order to avoid mirror effect

            cv::Point p1;
            p1 = approx_curve[1];
            approx_curve[1] = approx_curve[3];
            approx_curve[3] = p1;

            // compute distance and eventually swap corners
            computeDistanceandSwap(approx_curve);
            // code for printing map with corners
            /*
            for (int i = 0; i < approx_curve.size();
                 i++) // access by reference to avoid copying
            {

                switch (i)
                {
                case 0:
                    color = Scalar(0, 0, 255);
                    break;
                case 1:
                    color = Scalar(255, 0, 0);
                    break;
                case 2:
                    color = Scalar(0, 255, 0);
                    break;
                case 3:
                    color = Scalar(255, 255, 255);
                    break;
                default:
                    color = Scalar(0, 0, 255);
                    break;
                }
                cv::circle(contours_img, {approx_curve[i]}, 20, color, -1);
                imshow("corner points", contours_img);
            }*/
            //list of corners that identify the arena
            arena = approx_curve;
        }
    }
    return arena;
}

void reTransform(cv::Mat &persp_img, double &pixel_scale){
    // Destination image
    Size size(400, 600);
    Arena arena = Arena();
    arena.findArena(persp_img);
    std::vector<cv::Point> corners = arena.getCorners();

    Mat im_dst = Mat::zeros(size, CV_8UC3);
    // Create a vector of points.
    std::vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(25,25));
    pts_dst.push_back(Point2f(size.width-25, +25));
    pts_dst.push_back(Point2f(size.width-25, size.height-25));
    pts_dst.push_back(Point2f(25, size.height-25));

    Mat tform = findHomography(corners, pts_dst);
    warpPerspective(persp_img, im_dst, tform, size, cv::INTER_LINEAR,
                    cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    double top_dist = cv::norm(corners[0]-corners[1]);
    pixel_scale =top_dist/960;
    std::cout<<"pixel scale: "<<pixel_scale<<std::endl;

    persp_img = im_dst;
}


// Example of function to determine the perspective transformation of a
// rectangle on the ground plane (with manual intervention from the user, that
// is required to select the 4 corner points of the rectangle, starting from the
// top-left corner and proceeding in clockwise order, and the origin (top-left)
// and scale of the transformed top view image).
// Since the real size of the rectangle is known (width: 1m, height: 1.5m),
// the fucntion returns also the pixel_scale, i.e. the size (in mm) of each
// pixel in the top view image
Mat Inverse_Perspective_Mapping::findTransform(
    const std::string &calib_image_name, const cv::Mat &camera_matrix,
    const cv::Mat &dist_coeffs, double &pixel_scale, cv::Mat &persp_img)
{
    Mat calib_image, original_image = imread(calib_image_name);

    if (original_image.empty())
    {
        throw std::runtime_error("Could not open image " + calib_image_name);
    }

    undistort(original_image, calib_image, camera_matrix, dist_coeffs);

    // find corners
    std::vector<cv::Point> corners = findCorners(calib_image);

    // Destination image
    Size size(400, 600);
    Mat im_dst = Mat::zeros(size, CV_8UC3);
    // Create a vector of points.
    std::vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(5, 5));
    pts_dst.push_back(Point2f(size.width - 1, 5));
    pts_dst.push_back(Point2f(size.width - 1, size.height - 1));
    pts_dst.push_back(Point2f(5, size.height - 1));

    Mat tform = findHomography(corners, pts_dst);
    warpPerspective(calib_image, im_dst, tform, size, cv::INTER_LINEAR,
                    cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    
    reTransform(im_dst,pixel_scale);
    persp_img = im_dst;
    //imshow("Image", im_dst);
    //waitKey(0);
    return tform;
}


// Store all the parameters to a file, for a later use, using the FileStorage
// class methods
void Inverse_Perspective_Mapping::storeAllParameters(
    const std::string &filename, const cv::Mat &camera_matrix,
    const cv::Mat &dist_coeffs, double pixel_scale, const Mat &persp_transf)
{
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    fs << "camera_matrix" << camera_matrix << "dist_coeffs" << dist_coeffs
       << "pixel_scale" << pixel_scale << "persp_transf" << persp_transf;
    fs.release();
}

cv::Mat Inverse_Perspective_Mapping::run(std::string intrinsic_conf,
                                         std::string image,
                                         std::string outputfilename)
{
    cv::Mat camera_matrix, dist_coeffs;
    loadCoefficients(intrinsic_conf, camera_matrix, dist_coeffs);
    this->outputfilename = outputfilename;
    double pixel_scale = 0;
    Mat persp_img;
    Mat persp_transf =
        findTransform(image, camera_matrix, dist_coeffs, pixel_scale, persp_img);
    storeAllParameters(outputfilename, camera_matrix, dist_coeffs, pixel_scale,
                       persp_transf);
    return persp_img;
}
