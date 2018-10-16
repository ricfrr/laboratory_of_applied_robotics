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
    imshow("img", img);
    waitKey(0);
    Digit_Recognition dg_recognition = Digit_Recognition();
    //dg_recognition.set_filter(HSVFilterRange("bad"));
    std::vector<PeopleData> data = dg_recognition.detect_digits_for_map(img);
    std::vector<Circle> circles;

    for (int i = 0; i < data.size(); i++)
    {
        std::cout << "detected a guy called " << data[i].digit << " at <" << data[i].center.x << "," << data[i].center.y << "> with a radius of " << data[i].radius << std::endl;
        cv::Point center;
        center.x = data[i].center.x;
        center.y = data[i].center.y;
        Circle circle_d = Circle();
        circle_d.setDigit(data[i].digit);
        circle_d.setCenter(center);
        circle_d.setRadius(data[i].radius);
        circles.push_back(circle_d);
    }
}

std::vector<Circle> People::getCircles() { return circles; };
