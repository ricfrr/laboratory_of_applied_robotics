#include "../Headers/ExitPoint.hpp"

ExitPoint::ExitPoint()
{
    // creator
}
ExitPoint::~ExitPoint()
{
    // nada
}

void ExitPoint::findExitPoint(const Mat &img)
{
    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    // Preparing the kernel matrix
    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT, cv::Size((1 * 2) + 1, (1 * 2) + 1));

    // Definining contour containers and hierarchy to find the right contours
    cv::Mat contours_img;
    std::vector<std::vector<cv::Point>> contours, contours_approx;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point> approx_curve, corners;
    std::vector<cv::Point> arena;

    // Find blue regions
    cv::Mat blue_mask;
    cv::inRange(hsv_img, cv::Scalar(90, 50, 40), cv::Scalar(140, 255, 255), blue_mask);

    // Process blue mask

    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((2 * 2) + 1, (2 * 2) + 1));
    // Filter (applying dilation, blurring, dilation and erosion) the image
    cv::dilate(blue_mask, blue_mask, kernel);
    cv::GaussianBlur(blue_mask, blue_mask, cv::Size(9, 9), 6, 6);
    cv::dilate(blue_mask, blue_mask, kernel);
    cv::erode(blue_mask, blue_mask, kernel);
    

    // Process blue mask

    contours_img = img.clone();
    cv::findContours(blue_mask, contours, hierarchy, cv::RETR_LIST,
                     cv::CHAIN_APPROX_SIMPLE);
    drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
                 cv::LINE_AA);
    
    for (int i = 0; i < contours.size(); ++i)
    {
        approxPolyDP(contours[i], approx_curve, epsilon_approx, true);

        if (approx_curve.size() == 4)
        {
            corners = approx_curve;
            contours_approx = {approx_curve};
            drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
        }
    }
    setCorners(corners);
    corners = getCorners();
    std::cout << "Exit : " << corners << std::endl;

}
