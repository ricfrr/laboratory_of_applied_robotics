#include "../Headers/Obstacle.hpp"

Obstacle::Obstacle()
{
    // creator
}
Obstacle::~Obstacle()
{
    // nada
}
void Obstacle::findObstacles(const Mat &img)
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

    // Find red regions: h values around 0 (positive and negative angle: [0,15] U [160,179])
    cv::Mat red_mask_low, red_mask_high, red_mask;
    cv::inRange(hsv_img, cv::Scalar(0, 50, 50), cv::Scalar(10, 255, 255), red_mask_low);
    cv::inRange(hsv_img, cv::Scalar(160, 50, 50), cv::Scalar(179, 255, 255), red_mask_high);
    cv::addWeighted(red_mask_low, 1.0, red_mask_high, 1.0, 0.0, red_mask); // combine together the two binary masks

    // Filter (applying dilation, blurring, dilation and erosion) the image
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((1 * 2) + 1, (1 * 2) + 1));
    // Filter (applying an erosion and dilation) the image
    cv::erode(red_mask, red_mask, kernel);
    cv::dilate(red_mask, red_mask, kernel);

    // Process red mask

    contours_img = img.clone();
    cv::findContours(red_mask, contours, hierarchy, cv::RETR_LIST,
                     cv::CHAIN_APPROX_SIMPLE);
    drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
                 cv::LINE_AA);

    for (int i = 0; i < contours.size(); ++i)
    {
        approxPolyDP(contours[i], approx_curve, epsilon_approx, true);
        contours_approx = {approx_curve};
        if (approx_curve.size() == 3)
        {
            Triangle triangle = Triangle();
            triangle.setCorners(approx_curve);
            triangles.push_back(triangle);
            std::cout << "Triangle : " << triangle.getCorners() << std::endl;
            drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
        }
        else if (approx_curve.size() == 4)
        {
            Square square = Square();
            square.setCorners(approx_curve);
            squares.push_back(square);
            std::cout << "Square : " << square.getCorners() << std::endl;
            drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
        }
        else if (approx_curve.size() == 5)
        {   
            Pentagon pentagon = Pentagon();
            pentagon.setCorners(approx_curve);
            pentagons.push_back(pentagon);
            std::cout << "Pentagon : " << pentagon.getCorners() << std::endl;
            drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
        }
        else if (approx_curve.size() == 6)
        {
            Hexagon hexagon = Hexagon();
            hexagon.setCorners(approx_curve);
            hexagons.push_back(hexagon);
            std::cout << "Hexagon : " << hexagon.getCorners() << std::endl;
            drawContours(contours_img, contours_approx, -1, cv::Scalar(0, 170, 220), 3, cv::LINE_AA);
        }
    }
    //imshow("shapes", contours_img);
    //waitKey(0);
}

std::vector<Square *> Obstacle::getSquares()
{
    std::vector<Square *> mySquare;
    for(int i=0;i<squares.size();i++)
        mySquare.push_back(&squares[i]);

    return mySquare;

};

std::vector<Triangle *> Obstacle::getTriangles()
{
    std::vector<Triangle *> mytriangles;
    for(int i=0;i<triangles.size();i++)
        mytriangles.push_back(&triangles[i]);
    
    return mytriangles;
};

std::vector<Pentagon *> Obstacle::getPentagons()
{
    std::vector<Pentagon *> myPentagons;
    for(int i=0;i<pentagons.size();i++)
        myPentagons.push_back(&pentagons[i]);
    return myPentagons;
};

std::vector<Hexagon *> Obstacle::getHexagons()
{
    std::vector<Hexagon *> myHexagons;
    for(int i=0;i<hexagons.size();i++)
        myHexagons.push_back(&hexagons[i]);
    return myHexagons;
};
