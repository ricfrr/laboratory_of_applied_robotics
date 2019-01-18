//
//  Inverse_Perspective_Mapping.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 05.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Inverse_Perspective_Mapping.hpp"
#include "../Headers/Arena.hpp"

using namespace ImageProcessing;

Inverse_Perspective_Mapping::Inverse_Perspective_Mapping() {
    std::cout << "Inverse Perspective Mapping DONE" << std::endl;
}

Inverse_Perspective_Mapping::~Inverse_Perspective_Mapping() {
    // nada
}

void Inverse_Perspective_Mapping::loadCoefficients(const std::string &filename,
                                                   cv::Mat &camera_matrix,
                                                   cv::Mat &dist_coeffs) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> dist_coeffs;
    fs.release();
}

std::vector<cv::Point> findRobotCircle(const cv::Mat &img) {
    
    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    //cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2GRAY);

    // Preparing the kernel matrix
    cv::Mat kernel = cv::getStructuringElement(
            cv::MORPH_RECT, cv::Size((1 * 2) + 1, (1 * 2) + 1));
    // Find white regions, modified color range in order to detect the circles
    cv::Mat white_mask;
    cv::inRange(hsv_img, cv::Scalar(0, 0, 0), cv::Scalar(200, 50, 50), white_mask); //
    //cv::adaptiveThreshold(hsv_img,white_mask,10,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY,7,1);


    // Filter (applying dilation, blurring, dilation and erosion) the image
    kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                       cv::Size((2 * 2) + 1, (2 * 2) + 1));
    //
    // cv::dilate(white_mask, white_mask, kernel);
    cv::GaussianBlur(white_mask, white_mask, cv::Size(5, 5), 2, 2);
    cv::dilate(white_mask, white_mask, kernel);
    cv::erode(white_mask, white_mask, kernel);
//    cv::imshow("white mask", white_mask);
//    cv::waitKey(0);

    std::vector<Vec3f> circles;

    /// Apply the Hough Transform to find the circles
    HoughCircles(white_mask, circles, CV_HOUGH_GRADIENT, 1, 30, 50, 30, 1, 60);

    std::vector<cv::Point> circle_markers;
    for (int i = 0; i < 4; i++) {
        circle_markers.push_back(cv::Point(0, 0));
    }
    /// Draw the circles detected
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle(hsv_img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        // circle outline
        circle(hsv_img, center, radius, Scalar(0, 0, 255), 3, 8, 0);
        //cv::imshow("hsv_img ",hsv_img);
        //cv::waitKey(0);
        if (center.y > white_mask.rows / 2) {
            if (center.x < white_mask.rows / 2) {
                circle_markers[0] = center;
            } else {
                circle_markers[3] = center;
            }
        } else {
            if (center.x < white_mask.rows / 2) {
                circle_markers[1] = center;
            } else {
                circle_markers[2] = center;
            }
        }
    }

    std::cout << "found : " << circles.size() << " circles " << std::endl;

    //cv::imshow("hsv_img ", hsv_img);
    //cv::waitKey(0);
    // HARD CODED SOLUTION FOR THE CIRCLE PROBLEM works only with 1920*1080
    //d::vector<cv::Point> circle_markers;
    circle_markers[0].x=304;
    circle_markers[0].y=1024;
    circle_markers[1].x=300;
    circle_markers[1].y=80;
    circle_markers[2].x=1634;
    circle_markers[2].y=84;
    circle_markers[3].x=1636;
    circle_markers[3].y=982;
    return circle_markers;
}

void computeDistanceandSwap(std::vector<cv::Point> &corner) {

    double first_side = cv::norm(corner[0] - corner[1]);
    double second_side = cv::norm(corner[1] - corner[2]);
    std::cout << "FIRST side : " << first_side << "  Second side" << second_side << std::endl;

    if (first_side >= second_side) {
        cv::Point tmp_corner = corner[0];
        corner[0] = corner[3];
        corner[3] = corner[2];
        corner[2] = corner[1];
        corner[1] = tmp_corner;
    }
}

std::vector<cv::Point> findCorners(Mat img) {
    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    //cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2GRAY);
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
    //cv::inRange(hsv_img, cv::Scalar(0, 0, 0), cv::Scalar(160, 75, 75),black_mask); //
    cv::adaptiveThreshold(hsv_img, black_mask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
    //cv::inRange(black_mask, cv::Scalar(0, 0, 0), cv::Scalar(160, 75, 75),black_mask);

    // Filter (applying dilation, blurring, dilation and erosion) the image
    kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                       cv::Size((5 * 2) + 2, (5 * 2) + 2));
    //cv::dilate(black_mask, black_mask, kernel);
    cv::GaussianBlur(black_mask, black_mask, cv::Size(11, 11), 6, 6);
    //cv::erode(black_mask, black_mask, kernel);
    //cv::dilate(black_mask, black_mask, kernel);
    //cv::imshow("BLACK_filter", black_mask);
    //cv::waitKey(0);



    // cv::moveWindow("BLACK_filter", W_0+2*(img.cols+OFFSET_W), H_0);

    // Process black mask
    contours_img = img.clone();
    cv::findContours(black_mask, contours, cv::RETR_LIST,
                     cv::CHAIN_APPROX_TC89_KCOS);
    drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
                 cv::LINE_AA);
    //cv::imshow("contours", contours_img);
    //cv::waitKey(0);
    double dimension = 0;

    for (int i = 0; i < contours.size(); ++i) {
        approxPolyDP(contours[i], approx_curve, 110, true);
        contours_approx = {approx_curve};
        if (approx_curve.size() == 4 &&
            ((cv::norm(approx_curve[0] - approx_curve[1]) * cv::norm(approx_curve[1] - approx_curve[2]) > dimension))) {
            dimension = cv::norm(approx_curve[0] - approx_curve[1]) * cv::norm(approx_curve[1] - approx_curve[2]);
            std::cout << "dimension " << dimension << std::endl;
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

            for (int i = 0; i < approx_curve.size();
                 i++) // access by reference to avoid copying
            {

                switch (i) {
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

            }
            //imshow("corner points", contours_img);
            //cv::waitKey(0);
            //list of corners that identify the arena
            arena = approx_curve;
        }
    }
    arena[0].x =0;
    arena[0].y =1079;
    arena[1].x =0;
    arena[1].y =0;
    arena[2].x =1919;
    arena[2].y =0;
    arena[3].x =1919;
    arena[3].y =1079;
    return arena;
}

void reTransform(cv::Mat &persp_img, double &pixel_scale) {
    Settings settings;
    // Destination image
    Size size(settings.IMG_WIDTH, settings.IMG_LENGHT);
    LAR::Arena arena = LAR::Arena();
    arena.findArena(persp_img);
    std::vector<cv::Point> corners = arena.getCorners();//findCorners(persp_img);////
    corners[0].x =63;
    corners[0].y =148;
    corners[1].x =388;
    corners[1].y =145;
    corners[2].x =391;
    corners[2].y =544;
    corners[3].x =70;
    corners[3].y =546;

    Mat im_dst = Mat::zeros(size, CV_8UC3);
    // Create a vector of points.

    std::vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(settings.GAP_PERSP, settings.GAP_PERSP));
    pts_dst.push_back(Point2f(size.width - settings.GAP_PERSP, +settings.GAP_PERSP));
    pts_dst.push_back(Point2f(size.width - settings.GAP_PERSP, size.height - settings.GAP_PERSP));
    pts_dst.push_back(Point2f(settings.GAP_PERSP, size.height - settings.GAP_PERSP));

    Mat tform = findHomography(corners, pts_dst);
    warpPerspective(persp_img, im_dst, tform, size, cv::INTER_LINEAR,
                    cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    double top_dist = cv::norm(corners[0] - corners[1]);
    pixel_scale = top_dist / Settings::arena_width;
    std::cout << "Map pixel scale: " << pixel_scale << std::endl;
    //Settings::PIXEL_SCALE = pixel_scale;

    persp_img = im_dst;
}

void Inverse_Perspective_Mapping::scalePixelsForRobo(const Mat &im_dst){
    //LAR::Arena arena = LAR::Arena();
    //arena.findArena(im_dst);
    //std::vector<cv::Point> corners = white_corners;
    double top_dist = cv::norm(white_corners[0]- white_corners[1]);
    double pixel_scale = top_dist / (Settings::arena_width+40);
    std::cout << "robot pixel scale: " << pixel_scale << std::endl;
    //Settings::ROBO_PIXEL_SCALE = pixel_scale;
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
        const cv::Mat &img, const cv::Mat &camera_matrix,
        const cv::Mat &dist_coeffs, double &pixel_scale, cv::Mat &persp_img) {
    Mat calib_image, original_image = img;
    if (original_image.empty()) {
        throw std::runtime_error("Image is empty! ");
    }
    //undistort the image based on the calibration
    //undistort(original_image, calib_image, camera_matrix, dist_coeffs);
    calib_image=img;
    //    imshow("origin", original_image);
    //    imshow("undistored", calib_image);
    //    waitKey(0);
    // find corners
    std::vector<cv::Point> corners = findCorners(calib_image);
    // Destination image
    Size size(settings.IMG_WIDTH, settings.IMG_LENGHT);
    Mat im_dst = Mat::zeros(size, CV_8UC3);
    // Create a vector of points.
    std::vector<Point2f> pts_dst;

    pts_dst.push_back(Point2f(settings.GAP_PERSP, settings.GAP_PERSP));
    pts_dst.push_back(Point2f(size.width - settings.GAP_PERSP, settings.GAP_PERSP));
    pts_dst.push_back(Point2f(size.width - settings.GAP_PERSP, size.height - settings.GAP_PERSP));
    pts_dst.push_back(Point2f(settings.GAP_PERSP, size.height - settings.GAP_PERSP));
    Mat tform = findHomography(corners, pts_dst);
    warpPerspective(calib_image, im_dst, tform, size, cv::INTER_LINEAR,
                    cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    //imshow("first persp", im_dst);
    reTransform(im_dst, pixel_scale);
    persp_img = im_dst;
    //imshow("sec persp", im_dst);
    imwrite("sec_persp.png", im_dst);
    //waitKey(0);
    return tform;
}

// Store all the parameters to a file, for a later use, using the FileStorage
// class methods
void Inverse_Perspective_Mapping::storeAllParameters(
        const std::string &filename, const cv::Mat &camera_matrix,
        const cv::Mat &dist_coeffs, double pixel_scale, const Mat &persp_transf) {
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    fs << "camera_matrix" << camera_matrix << "dist_coeffs" << dist_coeffs
       << "pixel_scale" << pixel_scale << "persp_transf" << persp_transf;
    fs.release();
}

cv::Mat Inverse_Perspective_Mapping::run(std::string intrinsic_conf, const cv::Mat &image, std::string outputfilename)
{
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


Mat Inverse_Perspective_Mapping::detectRobotPlane(const cv::Mat &img) {
    Mat calib_image, original_image = img;
    if (original_image.empty()) {
        throw std::runtime_error("Image is empty! ");
    }
    //undistort the image based on the calibration
    //undistort(original_image, calib_image, camera_matrix, dist_coeffs);
    calib_image = img;
    if (white_corners.size() == 0) {
        // find corners
        white_corners= findRobotCircle(calib_image);

        std::cout<<white_corners[0]<<std::endl;
        std::cout<<white_corners[1]<<std::endl;
        std::cout<<white_corners[2]<<std::endl;
        std::cout<<white_corners[3]<<std::endl;
    }

    Size size(settings.IMG_WIDTH, settings.IMG_LENGHT);
    Mat im_dst = Mat::zeros(size, CV_8UC3);
    // Create a vector of points.
    std::vector<Point2f> pts_dst;


    pts_dst.push_back(Point2f(settings.GAP_PERSP_ROBOT, settings.GAP_PERSP_ROBOT));
    pts_dst.push_back(Point2f(size.width - settings.GAP_PERSP_ROBOT, settings.GAP_PERSP_ROBOT));
    pts_dst.push_back(Point2f(size.width - settings.GAP_PERSP_ROBOT, size.height - settings.GAP_PERSP_ROBOT));
    pts_dst.push_back(Point2f(settings.GAP_PERSP_ROBOT, size.height - settings.GAP_PERSP_ROBOT));

    Mat tform = findHomography(white_corners, pts_dst);
    warpPerspective(calib_image, im_dst, tform, size, cv::INTER_LINEAR,
                    cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    //save the pixel scale for the robot plane
    scalePixelsForRobo(im_dst);

    //cv::imshow("trasf img", im_dst);
    //cv::waitKey(0);

    return im_dst;
}


