#include "../Headers/Cell.hpp"

Cell::Cell()
{}
Cell::~Cell()
{
    // nada
}

std::vector<cv::Point> Cell::getCorners()
{
    std::vector<cv::Point> corners;
    corners.push_back(Cell::getTopLeft());
    corners.push_back(Cell::getTopRight());
    corners.push_back(Cell::getBottomRight());
    corners.push_back(Cell::getBottomLeft());
    return corners;
};
//
void Cell::findHalf(double &half_h, double &half_w, const std::vector<cv::Point> &corners)
{
    for (int i = 0; i < corners.size(); i++)
    {
        half_h = half_h + corners[i].y;
        half_w = half_w + corners[i].x;
    }
    half_h = half_h / corners.size();
    half_w = half_w / corners.size();
}

void Cell::setCorners(std::vector<cv::Point> corners)
{
    double half_h = 0;
    double half_w = 0;
    findHalf(half_h, half_w, corners);

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].x < half_w)
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
            if (corners[i].y < half_h)
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

cv::Point Cell::getTopLeft() { return top_left; };
void Cell::setTopLeft(cv::Point topLeft) { top_left = topLeft; };

cv::Point Cell::getTopRight() { return top_right; };
void Cell::setTopRight(cv::Point topRight) { top_right = topRight; };

cv::Point Cell::getBottomLeft() { return bottom_left; };
void Cell::setBottomLeft(cv::Point bottomLeft) { bottom_left = bottomLeft; };

cv::Point Cell::getBottomRight() { return bottom_right; };
void Cell::setBottomRight(cv::Point bottomRight) { bottom_right = bottomRight; };


// set content of cells 
void Cell::setEmpty()
{
    this->state = EMPTY;
    
    empty = true;
    exit_p = false;
    border = false;
    obstacle = false;
    rescue = false;
};
void Cell::setExit()
{
    empty = false;
    exit_p = true;
    border = false;
    obstacle = false;
    rescue = false;
};
void Cell::setBorder()
{
    empty = false;
    exit_p = false;
    border = true;
    obstacle = false;
    rescue = false;
};
void Cell::setObstacle()
{
    empty = false;
    exit_p = false;
    border = false;
    obstacle = true;
    rescue = false;
};
void Cell::set_Empty()
{
    setEmpty();
    
    for(int i=0;i<subcells.size();i++)
        subcells[i].set_Empty();
    
};
void Cell::set_Exit()
{
    setExit();
    
    for(int i=0;i<subcells.size();i++)
        if(subcells[i].state == MIXED || subcells[i].state == FULL)
            subcells[i].set_Exit();
};
void Cell::set_Border()
{
    setBorder();
    
    for(int i=0;i<subcells.size();i++)
        if(subcells[i].state == MIXED || subcells[i].state == FULL)
            subcells[i].set_Border();
};
void Cell::set_Obstacle()
{
    setObstacle();
    
    for(int i=0;i<subcells.size();i++)
        if(subcells[i].state == MIXED || subcells[i].state == FULL)
            subcells[i].set_Obstacle();
};

bool Cell::contains_object(){
    if(state == MIXED || state == FULL)
        return true;
    else if (!empty)
        return true;
    else
        return false;
}

int Cell::getDigit(){
    return digit;
};
void Cell::setRescue(int digit_i)
{
    empty = false;
    exit_p = false;
    border = false;
    obstacle = false;
    rescue = true;
    digit = digit_i;
};

bool Cell::isEmpty(cv::Point forPoint)
{
    if(state == EMPTY)
        return true;
    else if(state == FULL)
        return false;
    else {
        if(subcells.size() == 0)
            return false;
        
        bool result = true;
        
            
        Cell cell;
            
        bool first_half_v = forPoint.x <= subcells[0].getTopRight().x;
        bool first_half_h = forPoint.y <= subcells[0].getBottomLeft().y;
        
        int index;
        
        if(first_half_h && first_half_v)
            index = 0;
        else if(!first_half_v && first_half_h)
            index = 1;
        else if(!first_half_v && !first_half_h)
            index = 2;
        else if(first_half_v && !first_half_h)
            index = 3;
        
        result = subcells[index].isEmpty();
        
        return result;
    }
};
bool Cell::isExit()
{
    return exit_p;
};
bool Cell::isBorder()
{
    return border;
};
bool Cell::isObstacle()
{
    return obstacle;
};
bool Cell::isRescue()
{
    return rescue;
}

bool Cell::getState(){
    return this->state;
}

void Cell::refine_if_neccessary(std::vector<cv::Point> forShape){
    
    std::vector<cv::Point> shape;
    
    if(forShape.size() == 2){
        
        int radius = forShape[1].x;
        
        for(int i=0;i<360;i+=10){
            
            double rad = 2 * M_PI / 360;
            
            rad *= i;
            
            double x = forShape[0].x + sin(rad) * radius;
            double y = forShape[0].y + cos(rad) * radius;
            
            shape.push_back(cv::Point(x,y));
        }
        
    }else{
        shape = forShape;
    }
    
    findState(shape);
    
    switch (state) {
        case EMPTY:
            setEmpty();
            return;
        case MIXED:
            split(shape);
            break;
        case FULL:
            return;
            
        default:
            break;
    }
}

const std::vector<Cell*> Cell::getSubcells(){
    
    if(subcells.empty())
        return {};
    
    std::vector<Cell*> subs = { &subcells[0],&subcells[1],&subcells[2],&subcells[3] };
    return subs;
}

const std::vector<Cell*> Cell::getAllSubcells(){
    std::vector<Cell*> allcells;
    collectSubcells(allcells);
    return allcells;
}

void Cell::collectSubcells(std::vector<Cell*> &cells){
    
    for(int i=0;i<subcells.size();i++){
        subcells[i].collectSubcells(cells);
        Cell* cell = &subcells[i];
        cells.push_back(cell);
    }
    
}

double Cell::collision(std::vector<cv::Point> &withObject){
    
    
    return 0;
}

void Cell::findState(std::vector<cv::Point> contour){
    
//    cv::Mat contours_img(600,600,CV_8UC3,cv::Scalar(255, 255, 255));
//
//    std::vector<std::vector<cv::Point>> contours = {contour};
//    cv::drawContours(contours_img, contours, -1, cv::Scalar(40, 190, 40), 1,
//                 cv::LINE_AA);
//    cv::imshow("contour", contours_img);
//    cv::waitKey(0);
    
    std::vector<cv::Point> corners = getCorners();
    
    bool allIn = true;
    bool allOut = true;
    
    std::vector<double> results;
    for(int i=0;i<contour.size();i++){
        //check if contour points are inside the cell, outside the cell or on the border
        double result = cv::pointPolygonTest(corners, contour[i], true);
        //save the result
        results.push_back(result);
        //logic
        if(result < 0 && allIn)
            allIn = false;
        else if(result >= 0 && !allIn)
            allOut = false;
    }
    
    if(allIn)
        allOut = false;
    
    if(!allIn && !allOut)
        this->state = MIXED;
    else if (allIn)
        state_for_allIn(results);
    else if (allOut)
        state_for_allOut(contour);
}

void Cell::state_for_allIn(std::vector<double> results){
    
    bool all_points_on_border = true;
    
    for(int i=0;i<results.size();i++)
        if(results[i] != 0){
            all_points_on_border = false;
            break;
        }
    
    if(all_points_on_border)
        this->state = FULL;
    else
        this->state = MIXED;
}

void Cell::state_for_allOut(std::vector<cv::Point> contour){
    //check if every cell point is inside the contour
    std::vector<cv::Point> corners = getCorners();
    bool all_points_inside_contour = true;
    bool all_points_outside_contour = true;
    for(int i=0;i<corners.size();i++){
        double result = cv::pointPolygonTest(contour, corners[i], true);
        if(result < 0)
            all_points_inside_contour = false;
        if(result >= 0)
            all_points_outside_contour = false;
    }
    
    if(all_points_inside_contour)
        this->state = FULL;
    
    else if(all_points_outside_contour)
        this->state = EMPTY;
    
    else
        this->state = MIXED;
}

void Cell::split(std::vector<cv::Point> forShape){
    
    //std::cout << "splitting cell" << std::endl;
    
    Cell cell_1, cell_2, cell_3, cell_4;
    
    cv::Point topLeft       = top_left;
    cv::Point topCenter     = (top_right + top_left)*0.5;
    cv::Point topRight      = top_right;
    
    cv::Point leftCenter    = (top_left + bottom_left)*0.5;
    cv::Point center        = (top_left + bottom_right)*0.5;
    cv::Point rightCenter   = cv::Point(top_right.x, (top_right.y+bottom_right.y)/2);
    
    cv::Point bottomLeft    = bottom_left;
    cv::Point bottomCenter  = (bottom_right + bottom_left)*0.5;
    cv::Point bottomRight   = bottom_right;
    
    cell_1.setCorners({topLeft, topCenter, center, leftCenter});
    cell_2.setCorners({topCenter, topRight, rightCenter, center});
    cell_3.setCorners({center, rightCenter, bottomRight, bottomCenter});
    cell_4.setCorners({leftCenter, center, bottomCenter, bottomLeft});
    
    int cellArea = (topCenter.x - topLeft.x) * (leftCenter.y - topLeft.y);
    
    if(cellArea > 16){
        cell_1.refine_if_neccessary(forShape);
        cell_2.refine_if_neccessary(forShape);
        cell_3.refine_if_neccessary(forShape);
        cell_4.refine_if_neccessary(forShape);
    }
    
    this->subcells = {cell_1, cell_2, cell_3, cell_4};
    
    
//    findState({cv::Point(20,20),cv::Point(50,20),cv::Point(50,50),cv::Point(20,50)});
}
