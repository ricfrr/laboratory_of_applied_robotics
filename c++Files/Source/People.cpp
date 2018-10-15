#include "../Headers/People.hpp"

People::People()
{
    // creator
}
People::~People()
{
    // nada
}

void People::findCircles(const Mat &img)
{
    static const int W_0 = 300;
    static const int H_0 = 0;
    static const int OFFSET_W = 10;
    static const int OFFSET_H = 100;
    static const int max_radius = 70;

    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    // Preparing the kernel matrix,  Find green regions
    cv::Mat green_mask;
    cv::inRange(hsv_img, cv::Scalar(55, 90,55), cv::Scalar(70, 255, 255), green_mask);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((2 * 2) + 1, (2 * 2) + 1));
    // Filter (applying dilation, blurring, dilation and erosion) the image
    cv::dilate(green_mask, green_mask, kernel);
    cv::GaussianBlur(green_mask, green_mask, cv::Size(13, 13), 6, 6);
    cv::dilate(green_mask, green_mask, kernel);
    cv::erode(green_mask, green_mask, kernel);


    cv::imshow("GREEN_filter", green_mask);
    cv::moveWindow("GREEN_filter", W_0 + img.cols + OFFSET_W,
                   H_0 + img.rows + OFFSET_H);

    // Wait keypress
    cv::waitKey(0);
    // Blur image to improve performance of Hough detector
    std::vector<cv::Vec3f> circles_f;
    cv::HoughCircles(green_mask, circles_f, cv::HOUGH_GRADIENT, 1, 30, 50, 30, 0,
                     0); // Circle detection using Hough transform
    std::cout << "CIRCLES: " << circles_f.size() << std::endl;

    for (int i = 0; i < circles_f.size(); i++)
    {
        cv::Point center(cvRound(circles_f[i][0]), cvRound(circles_f[i][1]));
        int radius = cvRound(circles_f[i][2]);
        if (radius < max_radius)
        {
            // save center and radiust
            Circle circle = Circle();
            circle.setRadius(radius);
            std::cout << "Radius  :" << radius << std::endl;
            circle.setCenter(center);
            circles.push_back(circle);
            cv::circle(img, center, radius, cv::Scalar(0, 170, 220), 3, cv::LINE_AA,
                       0); // draw the circle outline
        }
    }
    imshow("Circle", img);
    cv::waitKey(0);
}

std::vector<Circle> People::getCircles() { return circles; };