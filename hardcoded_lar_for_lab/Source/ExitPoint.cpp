#include "../Headers/ExitPoint.hpp"

using namespace LAR;

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
    //cv::inRange(hsv_img, cv::Scalar(110, 80, 65), cv::Scalar(140, 250, 255), blue_mask);
    cv::inRange(hsv_img, cv::Scalar(90, 60, 55), cv::Scalar(130, 250, 255), blue_mask);

    // Process blue mask

    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((2 * 2) + 1, (2 * 2) + 1));
    // Filter (applying dilation, blurring, dilation and erosion) the image
    //cv::dilate(blue_mask, blue_mask, kernel);
    //cv::GaussianBlur(blue_mask, blue_mask, cv::Size(3, 3), 1, 1);
    
    cv::erode(blue_mask, blue_mask, kernel);
    cv::dilate(blue_mask, blue_mask, kernel);
   cv::erode(blue_mask, blue_mask, kernel);
   cv::erode(blue_mask, blue_mask, kernel);

   


    imshow("exit mask ", blue_mask);
    waitKey(0);

    // Process blue mask

    contours_img = img.clone();
    cv::findContours(blue_mask, contours, hierarchy, cv::RETR_LIST,
                     cv::CHAIN_APPROX_SIMPLE);
    drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
                 cv::LINE_AA);
    
    for (int i = 0; i < contours.size(); ++i)
    {
        approxPolyDP(contours[i], approx_curve, epsilon_approx, true);
        double area = cv::contourArea(contours[i]);
        
        
        if (approx_curve.size() == 4 && area > 2000)
        {
            corners = approx_curve;
            contours_approx = {approx_curve};
            drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
        }
    }
    setCorners(corners);
    corners = getCorners();
    imshow("exit",contours_img);
    waitKey(0);
    //std::cout << "Exit : " << corners << std::endl;

}

std::pair<cv::Point,double> ExitPoint::getEntryPoint(){

    //cv::Point center = top_left + (bottom_right - top_left)/2;
    cv::Point center = (top_left+top_right+bottom_left+bottom_right)/4;
    int height =  bottom_right.y - top_left.y;
    int width = bottom_right.x - top_left.x;

    //where is the exit in the map
    bool left = false;
    bool right = false;
    bool top = false;
    bool bottom = false;

    if(top_left.x < 50){
        left = true;
        //center.x += width/2;
    }
    else if(top_right.x > ImageProcessing::Settings().IMG_WIDTH -50){
        right = true;
        //center.x -= width/2;
    }

    if(top_left.y < 50)
        top = true;
    else if(bottom_left.y > ImageProcessing::Settings().IMG_LENGHT - 50)
        bottom = true;

    double angle = 0;

    if(height > width){
        //vertically
        if(left)
            angle = 1;
        else if(right)
            angle = 0;
        else
            std::runtime_error("ohh it's not so simple");
    }
    else{
        //horizontal
        if(top)
            angle = -0.5;
        else if(bottom)
            angle = 0.5;
        else
            std::runtime_error("ohh it's not so simple");
    }

    return std::pair<cv::Point,double> (center, angle * M_PI);
}
