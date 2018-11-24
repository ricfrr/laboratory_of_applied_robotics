#include "../Headers/Map.hpp"

Map::Map()
{
    // creator
}
Map::~Map()
{
    // nada
}


void Map::createMap(const Mat &img)
{

    arena.findArena(img);
    exit_point.findExitPoint(img);
    obstacles.findObstacles(img);
    people.findCircles(img);

    // detection of all obstacles
    initializeGrid(arena, exit_point, obstacles);
}

void Map::initializeGrid(Arena &arena, ExitPoint &exit_point,
                         Obstacle &obstacles)
{
    bool debug = false;
    
    // initalize cells with pixel coordinates and empty value
    int temp_x = 0;
    int temp_y = 0;
    int x_incr = map_pixel_w / n_col; // dimension of each cell in pixel
    int y_incr = map_pixel_h / n_row;
    for (int i = 0; i < n_row; i++)
    {
        std::vector<Cell*> temp_vec;
        for (int j = 0; j < n_col; j++)
        {

            Cell* cell = new Cell;
            // corners of the cell
            cv::Point top_left, top_right, bottom_left, bottom_right;
            top_left.x = temp_x;
            top_left.y = temp_y;
            top_right.x = temp_x + x_incr;
            top_right.y = temp_y;
            bottom_left.x = temp_x;
            bottom_left.y = temp_y + y_incr;
            bottom_right.x = temp_x + x_incr;
            bottom_right.y = temp_y + y_incr;
            std::vector<cv::Point> cell_corners;
            cell_corners.push_back(top_left);
            cell_corners.push_back(top_right);
            cell_corners.push_back(bottom_right);
            cell_corners.push_back(bottom_left);

            cell->setCorners(cell_corners);
            cell->setEmpty();
            // check if cell is in contact with an obj

            if (isOutofArena(cell_corners, arena)){
                
                arena.setCell(*cell);
                cell->setBorder();
            
//            std::vector<Cell*> cells;
//            getArenaCells(cells);
//            for(int i = 0;i<cells.size();i++)
//                std::cout <<"cell for arena "<< cells[i]->getTopLeft() << std::endl;
//            std::cout << "\n";
                
                if(debug)
                    std::cout << "b";
            
            }
            else
            {
                std::vector<cv::Point> corners = exit_point.getCorners();
                // std::cout << "CHECK EXIT" << std::endl;
                if (contact(cell_corners, corners))
                {
                    exit_point.setCell(*cell);
                    cell->setExit();
                    if(debug)
                        std::cout << "\033[1;34mx\033[0m";
                }
            }
            if (cell->isEmpty())
            {
                checkObstacles(*cell, obstacles);
                if (cell->isObstacle())
                {
                    if(debug)
                        std::cout << "\033[1;31mo\033[0m";
                }
            }
            if (cell->isEmpty())
            {
                checkPeople(*cell, people);
                if (cell->isRescue())
                {
                    if(debug)
                        std::cout << "\033[1;36mr\033[0m";
                }
            }
            if (cell->isEmpty())
            {
                if(debug)
                    std::cout << "\033[1;32me\033[0m";
            }
            temp_vec.push_back(cell);
            temp_x = temp_x + x_incr;
        }
        temp_y = temp_y + y_incr;
        temp_x = 0;
        if(debug)
            std::cout << std::endl;
        grid.push_back(temp_vec);
    }
    std::cout << "---- DONE ----" << std::endl;
};

void Map::getGrid(std::vector<std::vector<Cell*>> &grid){
    grid = this->grid;
}

Obstacle Map::getObstacles(){
    return this->obstacles;
}

People Map::getPeople(){
    return this->people;
}

ExitPoint Map::getExitPoint(){
    return this->exit_point;
}

double distanceBetweenTwoPoints(double x, double y, double a, double b)
{
    return sqrt(pow(x - a, 2) + pow(y - b, 2));
};

bool circleContact(std::vector<cv::Point> corners, Circle &circle)
{
    double distance;
    for (int i = 0; i < corners.size(); i++)
    {
        distance = distanceBetweenTwoPoints(corners[i].x, corners[i].y, circle.getCenter().x, circle.getCenter().y);
        if (distance < circle.getRadius())
        {
            return true;
        }
    }
    return false;
};

void Map::getPixelDimensions(int &width, int &height){
    width = this->map_pixel_w;
    height = this->map_pixel_h;
}

void Map::getArenaCells(std::vector<Cell *> &cells){
    cells = this->arena.getCell();
    
}

void Map::checkPeople(Cell &cell, People people)
{

    std::vector<Circle*> circles = people.getCircles();
    std::vector<cv::Point> cell_corners = cell.getCorners();
    //std::cout<<"circles size : "<<circles.size()<<" cell_corner : "<<cell_corners.size()<<std::endl;
    for (int i = 0; i < circles.size(); i++)
    {
        if (circleContact(cell_corners, *circles[i]))
        {
            cell.setRescue(circles[i]->getDigit());
            circles[i]->setCell(cell);

        }
    }
}

void Map::checkObstacles(Cell &cell, Obstacle &obstacles)
{
    bool touched = false;
    std::vector<Triangle> triangles = obstacles.getTriangles();
    std::vector<Square> squares = obstacles.getSquares();
    std::vector<Pentagon> pentagons = obstacles.getPentagons();
    std::vector<Hexagon> hexagons = obstacles.getHexagons();
    std::vector<cv::Point> cell_corners = cell.getCorners();
    if (!touched)
    {
        for (int k = 0; k < triangles.size(); k++)
        {
            std::vector<cv::Point> tmp_tr = triangles[k].getCorners();
            // std::cout << "CHECK TRIANGLE" << std::endl;
            if (contact(cell_corners, tmp_tr))
            {
                triangles[k].setCell(cell);
                touched = true;
            }
        }
    }
    if (!touched)
    {
        for (int k = 0; k < squares.size(); k++)
        {
            std::vector<cv::Point> tmp_sq = squares[k].getCorners();
            // std::cout << "CHECK SQUARE" << std::endl;
            if (contact(cell_corners, tmp_sq))
            {
                squares[k].setCell(cell);
                touched = true;
            }
        }
    }
    if (!touched)
    {
        for (int k = 0; k < pentagons.size(); k++)
        {
            std::vector<cv::Point> tmp_pt = pentagons[k].getCorners();
            // std::cout << "CHECK PENTAGON" << std::endl;
            if (contact(cell_corners, tmp_pt))
            {
                pentagons[k].setCell(cell);
                touched = true;
            }
        }
    }
    if (!touched)
    {
        for (int k = 0; k < hexagons.size(); k++)
        {
            std::vector<cv::Point> tmp_hx = hexagons[k].getCorners();
            // std::cout << "CHECK HEXAGON" << std::endl;
            if (contact(cell_corners, tmp_hx))
            {
                hexagons[k].setCell(cell);
                touched = true;
            }
        }
    }
    if (touched)
    {
        cell.setObstacle();
    }
};

bool Map::isOutofArena(std::vector<cv::Point> corners, Arena arena)
{
    cv::Point top_right = arena.getTopRight();
    cv::Point top_left = arena.getTopLeft();
    cv::Point bottom_left = arena.getBottomLeft();
    cv::Point bottom_right = arena.getBottomRight();

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].x >= top_right.x || corners[i].x >= bottom_right.x ||
            corners[i].x <= top_left.x || corners[i].x <= bottom_left.x)
        {
            return true;
        }
        else if (corners[i].y <= top_right.y || corners[i].y <= top_left.y ||
                 corners[i].y >= bottom_right.y ||
                 corners[i].y >= bottom_left.y)
        {
            return true;
        }
    }
    return false;
};

void Map::findMaxMinY(int &max_y, int &min_y,
                      const std::vector<cv::Point> poly)
{
    min_y = setting.IMG_LENGHT;
    max_y = 0;
    for (int i = 0; i < poly.size(); i++)
    {
        if (poly[i].y < min_y)
        {
            min_y = poly[i].y;
        }
        if (poly[i].y > max_y)
        {
            max_y = poly[i].y;
        }
    }
}

// check if a cell is in contact with a polygon,
bool Map::contact(std::vector<cv::Point> cell,
                  const std::vector<cv::Point> poly)
{
    int max_y, min_y;
    findMaxMinY(max_y, min_y, poly);
    // std::cout<<"poly size : "<<poly.size()<<" max_y : "<<max_y<<" min_y :
    // "<<min_y<<std::endl;
    for (int i = 0; i < cell.size(); i++)
    {
        cv::Point point = cell[i];
        double x = point.x;
        double y = point.y;

        unsigned short intersections = 0;
        for (int j = 0; j < poly.size(); j++)
        {
            cv::Point p1 = poly[j];
            // next point
            cv::Point p2;
            if (j + 1 != poly.size())
                p2 = poly[j + 1];
            else
                // if array ended than take first point
                p2 = poly[0];

            // if x is out bound not intersect
            if ((x <= p1.x && x <= p2.x) || (x > p1.x && x > p2.x))
                continue;
            if (y > max_y || y < min_y)
            {
                continue;
            }


            // where intersect at point x ?
            double y_int = p1.y + (p2.y - p1.y) * (x - p1.x) / (p2.x - p1.x);

            // if y_int is higher than y do intersect
            if (y <= y_int)
            {
                intersections++;
            }
        }
        /* is odd */
        if (intersections % 2)
        {
            return true;
        }
    }
    return false;
}

int Map::getImageWidth() {
    return map_pixel_w;
}

int Map::getImageHeight() {
    return map_pixel_h;
}

int Map::getGridColNum() {
    return n_col;
}

int Map::getGridRowNum() {
    return n_row;
}
