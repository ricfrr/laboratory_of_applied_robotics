#include "../Headers/Arena.hpp"

Arena::Arena()
{
    // creator
}
Arena::~Arena()
{
    // nada
}

void Arena::findArena(const Mat &img)
{
    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    // Preparing the kernel matrix
    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT, cv::Size((1 * 2) + 1, (1 * 2) + 1));

    // Definining contour containers and hierarchy to find the right contours
    cv::Mat contours_img;
    std::vector<std::vector<cv::Point>> contours, contours_approx_smallest;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point> approx_curve, corners;
    std::vector<cv::Point> arena;

    // Find black regions, modified color range in order to detect the shape
    cv::Mat black_mask;
    cv::inRange(hsv_img, cv::Scalar(0, 0, 0), cv::Scalar(220, 120, 120),
                black_mask); //

    // Filter (applying dilation, blurring, dilation and erosion) the image
    kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                       cv::Size((4 * 2) + 1, (4 * 2) + 1));
    cv::dilate(black_mask, black_mask, kernel);
    cv::GaussianBlur(black_mask, black_mask, cv::Size(9, 9), 6, 6);
    cv::dilate(black_mask, black_mask, kernel);
    cv::erode(black_mask, black_mask, kernel);

    // Process black mask

    contours_img = img.clone();
    cv::findContours(black_mask, contours, hierarchy, cv::RETR_LIST,
                     cv::CHAIN_APPROX_SIMPLE);
    drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
                 cv::LINE_AA);

    bool flag_first = true;
    int smallest_area = 0;

    for (int i = 0; i < contours.size(); ++i)
    {
        approxPolyDP(contours[i], approx_curve, epsilon_approx, true);

        if(approx_curve.size() == 4)
        {
            if (flag_first)
            {
                double a = contourArea(contours[i], false);
                smallest_area = a;
                corners = approx_curve;
                flag_first = false;
            }
            double a = contourArea(contours[i], false); // find the smallest area
            if (a < smallest_area)
            {
                smallest_area = a;
                corners = approx_curve;
            }
        }
        // drawContours(contours_img, contours_approx_smallest, -1, cv::Scalar(0,
        // 170, 220), 5, cv::LINE_AA);
    }

    std::cout << "Arena : " << corners << std::endl;
    //
    setCorners(corners);
    corners = getCorners();
    Scalar color = Scalar(0, 0, 255);
    for (int i = 0; i < corners.size();
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
        cv::circle(contours_img, {corners[i]}, 20, color, -1);
    }

    
    //imshow("arena", contours_img);
    cv::waitKey(0);

}

std::vector<cv::Point> Arena::getCorners()
{
    std::vector<cv::Point> corners;
    corners.push_back(Arena::getTopLeft());
    corners.push_back(Arena::getTopRight());
    corners.push_back(Arena::getBottomRight());
    corners.push_back(Arena::getBottomLeft());
    return corners;
};
//
void Arena::setCorners(std::vector<cv::Point> corners)
{
    int half_h = 300;
    int half_w = 200;
    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].x< half_w)
        {
            if (corners[i].y < half_h)
            {
                setTopLeft(corners[i]);
            }
            else
            {
                setBottomLeft(corners[i]);
            }
        }
        else
        {
            if (corners[i].y< half_h)
            {
                setTopRight(corners[i]);
            }
            else
            {
                setBottomRight(corners[i]);
            }
        }
    }
};

cv::Point Arena::getTopLeft() { return top_left; };
void Arena::setTopLeft(cv::Point topLeft) { top_left = topLeft; };

cv::Point Arena::getTopRight() { return top_right; };
void Arena::setTopRight(cv::Point topRight) { top_right = topRight; };

cv::Point Arena::getBottomLeft() { return bottom_left; };
void Arena::setBottomLeft(cv::Point bottomLeft) { bottom_left = bottomLeft; };

cv::Point Arena::getBottomRight() { return bottom_right; };
void Arena::setBottomRight(cv::Point bottomRight) { bottom_right = bottomRight; };
