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
    //imshow("img", img);
    //waitKey(0);
    Digit_Recognition dg_recognition = Digit_Recognition();
    
    std::vector<PeopleData> data = dg_recognition.detect_digits_for_map(img);
    for (int i = 0; i < data.size(); i++)
    {
        std::string conf;
        
        if(data[i].confidence > 75)
            conf = " with high confidence";
        else if (data[i].confidence > 50)
            conf = " with medium confidence";
        else if (data[i].confidence > 25)
            conf = " with low confidence";
        else
            conf = " with extremely low confidence";
            
        std::cout << "detected a guy called " << data[i].digit << " at <" << data[i].center.x << "," << data[i].center.y << "> with a radius of " << data[i].radius << conf << std::endl;
        cv::Point center;
        center.x = data[i].center.x;
        center.y = data[i].center.y;
        Circle circle_d = Circle();
        circle_d.setDigit(data[i].digit);
        circle_d.setCenter(center);
        circle_d.setRadius(data[i].radius/2);
        circles.push_back(circle_d);
    }
}

std::vector<Circle> People::getCircles() { return circles; };
