#include "../Headers/Cell.hpp"

Cell::Cell()
{
    setEmpty();
}
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


void Cell::setMixed(){
    this->state = MIXED;
    
    empty = false;
}
void Cell::setFull(){
    this->state = FULL;
    
    empty = false;
}

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
    border = true;
    exit_p = false;
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
    
    if(isEmpty())
        setFull();
    
    for(int i=0;i<subcells.size();i++)
        if(subcells[i].state == MIXED || subcells[i].state == FULL)
            subcells[i].set_Border();
        else{
            subcells[i].setFull();
            subcells[i].set_Border();
        }
        
};


void Cell::set_Obstacle()
{
    if(state != EMPTY)
        setObstacle();
    
    for(int i=0;i<subcells.size();i++)
        if(subcells[i].state != EMPTY){
            subcells[i].set_Obstacle();
            if(subcells[i].subcells.empty() && subcells[i].state != FULL)
                subcells[i].setFull();
                }
    
    if(state != EMPTY && state != MIXED && state != FULL)
        std::runtime_error("che cazzo");
};

void Cell::print(){
    std::cout <<
                "\nPrinting information about Cell\n" <<
                "Center: " << center() <<
                "\ntop left     point: " << top_left <<
                "\nbottom right point: " << bottom_right <<
                "\n" <<
                "\nempty-------------> " << empty <<
                "\nobstacle----------> " << obstacle <<
                "\nrescue------------> " << rescue <<
                "\nexit--------------> " << exit_p <<
                "\nis_Obstacle-------> " << isObstacle() <<
                "\nisRescue----------> " << isRescue() <<
                "\ncontains_object---> " <<contains_object() <<
                "\nisEmpty-----------> " <<isEmpty() <<
    "\ndone printing information\n" << std::endl;
}

bool Cell::contains_object(){
    if(state == MIXED || state == FULL)
        return true;
    else if (state!=EMPTY)
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

void Cell::set_Rescue(int digit_i){
    
    setRescue(digit_i);
    
    for(int i=0;i<subcells.size();i++)
        if(subcells[i].state == MIXED || subcells[i].state == FULL)
            subcells[i].set_Rescue(digit_i);
}

bool Cell::isEmpty(cv::Point forPoint)
{
    if(state == EMPTY && !subcells.empty())
        std::runtime_error("che cazzo");
    
    if(state == EMPTY && subcells.empty())
        return true;
    else if(state == FULL)
        return false;
    else if(forPoint.x >= 0 && forPoint.y >= 0) {
        
        if(subcells.empty())
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
    else
        return false;
};

bool Cell::isExit()
{
    return exit_p;
};
bool Cell::isBorder(cv::Point forPoint)
{
    
    if(isExit())
        return  false;
    
    if(state == EMPTY)
        return false;
    else if(state == FULL && border)
        return true;
    else {
        if(subcells.size() == 0 && border)
            return true;
        else if(subcells.size() == 0)
            return false;
        
        bool result = false;
        
        
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
        
        result = subcells[index].isBorder();
        
        return result;
    }
    
    return border;
};
bool Cell::isObstacle(cv::Point forPoint)
{
     if(isExit())
         return false;
    
    if(isEmpty())
        return false;
    else if(state == FULL && obstacle)
        return true;
    else if (!subcells.empty()){
        bool result = false;
        
        cv::Point point = forPoint;
        
        if(forPoint.x < 0 || forPoint.y < 0)
            point = center();
        
        Cell cell;
        
        bool first_half_v = point.x <= subcells[0].getTopRight().x;
        bool first_half_h = point.y <= subcells[0].getBottomLeft().y;
        
        int index;
        
        if(first_half_h && first_half_v)
            index = 0;
        else if(!first_half_v && first_half_h)
            index = 1;
        else if(!first_half_v && !first_half_h)
            index = 2;
        else if(first_half_v && !first_half_h)
            index = 3;
        
        result = subcells[index].isObstacle(point);
        
        if(!result && obstacle){
       //     std::cout << "PROBLEM" << std::endl;
            
            return true;
        }
        
        return result;
    }
    
    return obstacle;
};
bool Cell::isRescue()
{
    return rescue;
}

bool Cell::getState(){
    return this->state;
}

bool Cell::pointInside(cv::Point point){
    
    bool top = point.y >= top_left.y;
    bool left = point.x >= top_left.x;
    bool right = point.x <= top_right.x;
    bool bottom = point.y <= bottom_left.y;
    
    return top && left && right && bottom;
}

void Cell::refine_if_neccessary(std::vector<cv::Point> forShape){
    
    
    findState(forShape);
    
    switch (state) {
        case EMPTY:
            if(subcells.empty())
                setEmpty();
            else
                setMixed();
            break;
        case MIXED:
            split(forShape);
            break;
        case FULL:
            break;
            
        default:
            std::cout << "default" << std::endl;
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
    
    std::vector<cv::Point> shape;
    
    if(contour.size() == 2){
        
        int radius = contour[1].x;
        
        for(int i=0;i<360;i+=10){
            
            double rad = 2 * M_PI / 360;
            
            rad *= i;
            
            double x = contour[0].x + sin(rad) * radius;
            double y = contour[0].y + cos(rad) * radius;
            
            shape.push_back(cv::Point(x,y));
        }
        
    }else{
        shape = contour;
    }
    
    std::vector<cv::Point> corners = getCorners();
    
    bool allIn = true;
    bool allOut = true;
    
    std::vector<double> results;
    for(int i=0;i<shape.size();i++){
        //check if contour points are inside the cell, outside the cell or on the border
        double result = cv::pointPolygonTest(corners, shape[i], true);
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
        setMixed();
    else if (allIn)
        state_for_allIn(results);
    else if (allOut)
        state_for_allOut(shape);
}

void Cell::state_for_allIn(std::vector<double> results){
    
    bool all_points_on_border = true;
    
    for(int i=0;i<results.size();i++)
        if(results[i] != 0){
            all_points_on_border = false;
            break;
        }
    
    if(all_points_on_border)
        setFull();
    else
        setMixed();
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
        setFull();
    
    else if(all_points_outside_contour)
        setEmpty();
    
    else
        setMixed();
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
    
    //clone the cell into subcells
    cell_1.obstacle = obstacle;
    cell_1.exit_p = exit_p;
    cell_1.empty = empty;
    cell_1.rescue = rescue;
    cell_1.border = border;
    
    cell_2.obstacle = obstacle;
    cell_2.exit_p = exit_p;
    cell_2.empty = empty;
    cell_2.rescue = rescue;
    cell_2.border = border;
    
    cell_3.obstacle = obstacle;
    cell_3.exit_p = exit_p;
    cell_3.empty = empty;
    cell_3.rescue = rescue;
    cell_3.border = border;
    
    cell_4.obstacle = obstacle;
    cell_4.exit_p = exit_p;
    cell_4.empty = empty;
    cell_4.rescue = rescue;
    cell_4.border = border;
    
    int cellArea = (topCenter.x - topLeft.x) * (leftCenter.y - topLeft.y);
    
    if(cellArea > 16){
        cell_1.refine_if_neccessary(forShape);
        cell_2.refine_if_neccessary(forShape);
        cell_3.refine_if_neccessary(forShape);
        cell_4.refine_if_neccessary(forShape);
    }
    else{
        cell_1.findState(forShape);
        cell_2.findState(forShape);
        cell_3.findState(forShape);
        cell_4.findState(forShape);
    }
    
    this->subcells = {cell_1, cell_2, cell_3, cell_4};
    
    
//    findState({cv::Point(20,20),cv::Point(50,20),cv::Point(50,50),cv::Point(20,50)});
}

double Cell::width(){
    
    return  top_right.x - top_left.x;
}

double Cell::height(){
    return bottom_left.y - top_left.y;
}

cv::Point Cell::center(){
    
    return cv::Point(
                     top_left.x + width()/2,
                     top_left.y + height()/2
    );
}

bool Cell::isChildOf(Cell* &cell){
    
    std::vector<Cell*> subcells = cell->getAllSubcells();
    
    if(subcells.empty())
        return false;
    
    if (std::find(subcells.begin(), subcells.end(), this) != subcells.end())
        return true;
    
//    for(int i=0; i<subcells.size(); i++)
//        if(subcells[i]->center() == this->center())
//            return true;
    
    return false;

}
