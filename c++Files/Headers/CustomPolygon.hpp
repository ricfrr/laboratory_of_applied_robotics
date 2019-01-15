#ifndef CustomPolygon_hpp
#define CustomPolygon_hpp
#include <vector>

#include "Polygon.hpp"

using namespace cv;

/**
 \brief contains Geometries, such as Pentagons, Circles and other 2D Shapes
 */
namespace Geometry2D
{
    /**
     \brief Class for handling hexagon obstacles
     */
    class CustomPolygon : public Polygon
    {

    public:
        /*!
         * constructor of the CustomPolygon class
         */
        CustomPolygon();

        CustomPolygon(std::vector<cv::Point> points);

        /*!
         * destructor of the CustomPolygon class
         */
        ~CustomPolygon();

        /*!
         * return a list of corners of the CustomPolygon
         * @return list of corners of the CustomPolygon
         */
        std::vector<cv::Point> getCorners();
        /*!
         * set the list of corners
         * @param corners list of corners
         */
        void setCorners(std::vector<cv::Point> corners);

        void assign_points();


    private:
        std::vector<cv::Point> corners;
    };
}
#endif /* Hexagon_hpp */
