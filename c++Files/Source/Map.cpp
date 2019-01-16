#include "../Headers/Map.hpp"

Map::Map(const DigitRecognitionAlgo &algorithm, const unsigned int &suff_confidence, const unsigned int &search_angle, const double &d_angle, const unsigned int &extra_eroding) {
    // creator
    this->people = Digit_Recognition::PeopleStorage(algorithm,suff_confidence,search_angle,d_angle,extra_eroding);
    
//    map_pixel_w = Settings::IMG_WIDTH;
//    map_pixel_h = Settings::IMG_LENGHT;
}

Map::~Map() {
    for( auto &&row : grid){
        for(auto &&cell : row)
            delete cell;
    }
}

bool Map::wasSuccess(){
    return success;
}

void Map::clipPoints() {
    int radius = 122*robo->map_pixelscale;// 12,20 cm

    std::vector<cv::Point> tmp_point;
    std::vector<cv::Point> tmp_clip;
    //clipping exit-point
    tmp_point = exit_point.getCorners();
    tmp_clip = clipper.clip(tmp_point, radius);
    exit_point.setClippedCorners(tmp_clip);

    std::vector<Triangle *> triangles = obstacles.getTriangles();
    std::vector<Square *> squares = obstacles.getSquares();
    std::vector<Pentagon *> pentagons = obstacles.getPentagons();
    std::vector<Hexagon *> hexagons = obstacles.getHexagons();
    std::vector<CustomPolygon *> customPolygons= obstacles.getCustomPolygons();


    for (auto &triangle : triangles) {
        tmp_point = triangle->getCorners();
        tmp_clip = clipper.clip(tmp_point, radius);
        triangle->setClippedCorners(tmp_clip);
    }
    for (auto &square : squares) {
        tmp_point = square->getCorners();
        tmp_clip = clipper.clip(tmp_point, radius);
        square->setClippedCorners(tmp_clip);
    }
    for (auto &pentagon : pentagons) {
        tmp_point = pentagon->getCorners();
        tmp_clip = clipper.clip(tmp_point, radius);
        pentagon->setClippedCorners(tmp_clip);
    }
    for (auto &hexagon : hexagons) {
        tmp_point = hexagon->getCorners();
        tmp_clip = clipper.clip(tmp_point, radius);
        hexagon->setClippedCorners(tmp_clip);
    }
    for (auto &customPolygon : customPolygons) {
        tmp_point = customPolygon->getCorners();
        tmp_clip = clipper.clip(tmp_point, radius);
        customPolygon->setClippedCorners(tmp_clip);
    }

    tmp_point = arena.getCorners();
    tmp_clip = clipper.clipArena(tmp_point, 0);
    arena.setClippedCorners(tmp_clip);


}


void Map::createMap(const Mat &img,const Mat &robot_plane) {
   
    std::cout << "\nCREATING MAP\n" << std::endl;
    
    std::cout << "detect areana     ->";
    arena.findArena(img);
    std::cout << " done" << std::endl;
    
    std::cout << "detect exit       ->";
    exit_point.findExitPoint(img);
    std::cout << " done" << std::endl;
    
    std::cout << "find obstacles    ->";
    obstacles.findObstacles(img);
    std::cout << " done" << std::endl;
    
    std::cout << "find people:\n";
    people.findCircles(img);
    if(!people.success){
        std::cout << "could not find all digits!!! Try to modify parameters\n\n";
        success = false;
    }
    
    std::cout << "find robot        ->";
    this->robo = new Robot;
    
    this->robo->scalePixelsForRobo(robot_plane);
    this->robo->scalePixelsForMap(img);
    
    robo->findRobot(robot_plane);
    robo->initialPosition = robo->getPosition();
    robo->initialAngle = robo->angle;
    std::cout << " position in map: (" << robo->initialPosition.x << "," << robo->initialPosition.y << ")";
    std::cout << " done" << std::endl;
    
    std::cout << "clip points       ->";
    clipPoints();
    std::cout << " done" << std::endl;


    std::cout << "init grid:        ->";
    initializeGrid(arena, exit_point, obstacles);
    std::cout << " done" << std::endl;  
    
    std::cout << "\nCREATED MAP\n" << std::endl;
}

void Map::initializeGrid(Arena &arena, ExitPoint &exit_point,
                         Obstacle &obstacles) {
    bool debug = false;

    // initalize cells with pixel coordinates and empty value
    int temp_x = 0;
    int temp_y = 0;
    int x_incr = map_pixel_w / n_col; // dimension of each cell in pixel
    int y_incr = map_pixel_h / n_row;
    for (int i = 0; i < n_row; i++) {
        std::vector<Cell *> temp_vec;
        for (int j = 0; j < n_col; j++) {

            Cell *cell = new Cell;
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
            cell->set_Empty();
            // check if cell is in contact with an obj

            if (isOutofArena(cell_corners, arena)) {

                arena.setCell(*cell);
                cell->refine_if_neccessary(arena.getClippedCorners());
                cell->set_Border();


//            std::vector<Cell*> cells;
//            getArenaCells(cells);
//            for(int i = 0;i<cells.size();i++)
//                std::cout <<"cell for arena "<< cells[i]->getTopLeft() << std::endl;
//            std::cout << "\n";

                if (debug)
                    std::cout << "b";

            } else {
                std::vector<cv::Point> corners = exit_point.getClippedCorners();
                // std::cout << "CHECK EXIT" << std::endl;
                if (contact(cell_corners, corners)) {
                    exit_point.setCell(*cell);
                    cell->set_Exit();
                    cell->refine_if_neccessary(exit_point.getClippedCorners());

                    if (debug)
                        std::cout << "\033[1;34mx\033[0m";
                }
            }
            if (cell->isEmpty()) {
                checkObstacles(*cell, obstacles);
                if (cell->isObstacle()) {
                    if (debug)
                        std::cout << "\033[1;31mo\033[0m";
                }
            }
            if (cell->isEmpty()) {
                checkPeople(*cell, people);
                if (cell->isRescue()) {
                    if (debug)
                        std::cout << "\033[1;36mr\033[0m";
                }
            }
            if (cell->isEmpty()) {
                if (debug)
                    std::cout << "\033[1;32me\033[0m";
            }
            temp_vec.push_back(cell);
            temp_x = temp_x + x_incr;
        }
        temp_y = temp_y + y_incr;
        temp_x = 0;
        if (debug)
            std::cout << std::endl;
        grid.push_back(temp_vec);

    }
//std::cout << "---- DONE ----" << std::endl;
    
    if(grid.empty())
        success = false;
};

void Map::getGrid(std::vector<std::vector<Cell *>> &grid) {
    grid = this->grid;
}

Cell * Map::getCell(cv::Point forPoint){
    
    Cell* cell = new Cell;
    
    double x_ratio = getImageWidth() / getGridColNum();
    double y_ratio = getImageHeight() / getGridRowNum();
    
    int grid_row_check = (int) round(forPoint.y / y_ratio);
    int grid_col_check = (int) round(forPoint.x / x_ratio);
    
    while(grid_row_check >= (int)grid.size())
        grid_row_check -= 1;
    while(grid_col_check >= (int)grid[0].size())
        grid_col_check -= 1;
    
    if(grid_col_check < 0 || grid_row_check < 0)
        return cell;
    
    Cell* potential_cell = grid[grid_row_check][grid_col_check];
    
    if(potential_cell->pointInside(forPoint)){
        //std::cout << "found the right point" << std::endl;
        return potential_cell;
    }else{
        int delta_x_l = potential_cell->getTopLeft().x - forPoint.x;
        int delta_x_r = potential_cell->getTopRight().x - forPoint.x;
        int delta_y_t = potential_cell->getTopLeft().y - forPoint.y;
        int delta_y_b = potential_cell->getBottomLeft().y - forPoint.y;
        
        if(delta_x_l > 0 && grid_col_check > 0)
            grid_col_check -= 1;
        else if(delta_x_r < 0 && grid_col_check < grid[0].size()-1)
            grid_col_check += 1;
        
        if(delta_y_t > 0 && grid_row_check > 0)
            grid_row_check -= 1;
        else if(delta_y_b < 0 && grid_row_check < grid.size()-1)
            grid_row_check += 1;
        
        potential_cell = grid[grid_row_check][grid_col_check];
    }
    
    if(potential_cell->pointInside(forPoint)){
        //std::cout << "found the right point - after correction" << std::endl;
        return potential_cell;
    }
    else{
        //std::cout << "could not find the cell for the point !!!" << std::endl;
        return potential_cell;
    }
    
    return cell;
    
}

Obstacle Map::getObstacles() {
    return this->obstacles;
}

ImageProcessing::Digit_Recognition::PeopleStorage Map::getPeople() {
    return this->people;
}

ExitPoint Map::getExitPoint() {
    return this->exit_point;
}

double Map::distanceBetweenTwoPoints(double x, double y, double a, double b) {
    return sqrt(pow(x - a, 2) + pow(y - b, 2));
};

bool Map::circleContact(std::vector<cv::Point> corners, Circle *circle) {
    double distance;
    for (int i = 0; i < corners.size(); i++) {
        distance = distanceBetweenTwoPoints(corners[i].x, corners[i].y, circle->getCenter().x, circle->getCenter().y);
        if (distance < circle->getRadius()) {
            return true;
        }
    }
    return false;
};

void Map::getPixelDimensions(int &width, int &height) {
    width = this->map_pixel_w;
    height = this->map_pixel_h;
}

void Map::getArenaCells(std::vector<Cell *> &cells) {
    cells = this->arena.getCell();
}

void Map::checkPeople(Cell &cell, Digit_Recognition::PeopleStorage &people) {


    std::vector<cv::Point> cell_corners = cell.getCorners();

    for (int i = 0; i < people.people.size(); i++) {
        People guy = people.people[i];

        if (circleContact(cell_corners, &guy)) {
            cell.set_Rescue(people.people[i].name);
            cell.refine_if_neccessary({people.people[i].center, cv::Point(people.people[i].radius, 0)});
            // if(cell.isEmpty())

                // std::cout << "what!" << std::endl;
            people.people[i].setCell(cell);

        }
    }
}

void Map::checkObstacles(Cell &cell, Obstacle &obstacles) {

    std::vector<Triangle *> triangles = obstacles.getTriangles();
    std::vector<Square *> squares = obstacles.getSquares();
    std::vector<Pentagon *> pentagons = obstacles.getPentagons();
    std::vector<Hexagon *> hexagons = obstacles.getHexagons();
    std::vector<CustomPolygon *> customPolygons = obstacles.getCustomPolygons();

    std::vector<cv::Point> cell_corners = cell.getCorners();

        for (int k = 0; k < triangles.size(); k++) {
            std::vector<cv::Point> tmp_tr = triangles[k]->getClippedCorners();
            // std::cout << "CHECK TRIANGLE" << std::endl;
            if (contact(cell_corners, tmp_tr)) {
                cell.set_Obstacle();
                cell.refine_if_neccessary(triangles[k]->getClippedCorners());
                triangles[k]->setCell(cell);
                std::vector<Cell*> subcells = cell.getAllSubcells();
                
                for(int i=0;i<subcells.size();i++){
                    if(!subcells[i]->isObstacle() && subcells[i]->getSubcells().empty() && !subcells[i]->isEmpty()){
                        subcells[i]->set_Obstacle();
                    }
                }
                break;
            }
        }
    

        for (int k = 0; k < squares.size(); k++) {
            std::vector<cv::Point> tmp_sq = squares[k]->getClippedCorners();
            // std::cout << "CHECK SQUARE" << std::endl;
            if (contact(cell_corners, tmp_sq)) {
                cell.set_Obstacle();
                cell.refine_if_neccessary(squares[k]->getClippedCorners());
                squares[k]->setCell(cell);
                std::vector<Cell*> subcells = cell.getAllSubcells();
                
                for(int i=0;i<subcells.size();i++){
                    if(!subcells[i]->isObstacle() && subcells[i]->getSubcells().empty() && !subcells[i]->isEmpty()){
                        subcells[i]->set_Obstacle();
                    }
                }
                break;
            }
        }

        for (int k = 0; k < pentagons.size(); k++) {
            std::vector<cv::Point> tmp_pt = pentagons[k]->getClippedCorners();
            // std::cout << "CHECK PENTAGON" << std::endl;
            if (contact(cell_corners, tmp_pt)) {
                cell.set_Obstacle();
                cell.refine_if_neccessary(tmp_pt);
                pentagons[k]->setCell(cell);
                
                std::vector<Cell*> subcells = cell.getAllSubcells();
                
                for(int i=0;i<subcells.size();i++){
                    if(!subcells[i]->isObstacle() && subcells[i]->getSubcells().empty() && !subcells[i]->isEmpty()){
                        subcells[i]->set_Obstacle();
                    }
                }
                
                break;
            }
        }

        for (int k = 0; k < hexagons.size(); k++) {
            std::vector<cv::Point> tmp_hx = hexagons[k]->getClippedCorners();
            // std::cout << "CHECK HEXAGON" << std::endl;
            if (contact(cell_corners, tmp_hx)) {
                cell.set_Obstacle();
                cell.refine_if_neccessary(hexagons[k]->getClippedCorners());
                hexagons[k]->setCell(cell);
                std::vector<Cell*> subcells = cell.getAllSubcells();
                
                for(int i=0;i<subcells.size();i++){
                    if(!subcells[i]->isObstacle() && subcells[i]->getSubcells().empty() && !subcells[i]->isEmpty()){
                        subcells[i]->set_Obstacle();
                    }
                }
                break;
            }
        }

    for (int k = 0; k < customPolygons.size(); k++) {
        std::vector<cv::Point> tmp_cust = customPolygons[k]->getClippedCorners();
        // std::cout << "CHECK HEXAGON" << std::endl;
        if (contact(cell_corners, tmp_cust)) {
            cell.set_Obstacle();
            cell.refine_if_neccessary(customPolygons[k]->getClippedCorners());
            customPolygons[k]->setCell(cell);
            std::vector<Cell*> subcells = cell.getAllSubcells();

            for(int i=0;i<subcells.size();i++){
                if(!subcells[i]->isObstacle() && subcells[i]->getSubcells().empty() && !subcells[i]->isEmpty()){
                    subcells[i]->set_Obstacle();
                }
            }
            break;
        }
    }

};

bool Map::isOutofArena(std::vector<cv::Point> corners, Arena arena) {
    cv::Point top_left = arena.getClippedCorners()[0]; //top_left
    cv::Point top_right = arena.getClippedCorners()[1]; //top_right
    cv::Point bottom_right = arena.getClippedCorners()[2]; //bottom_right
    cv::Point bottom_left = arena.getClippedCorners()[3]; // bottom_left

    for (int i = 0; i < corners.size(); i++) {
        if (corners[i].x >= top_right.x || corners[i].x >= bottom_right.x ||
            corners[i].x <= top_left.x || corners[i].x <= bottom_left.x) {
            return true;
        } else if (corners[i].y <= top_right.y || corners[i].y <= top_left.y ||
                   corners[i].y >= bottom_right.y ||
                   corners[i].y >= bottom_left.y) {
            return true;
        }
    }
    return false;
};

void Map::findMaxMinY(int &max_y, int &min_y,
                      const std::vector<cv::Point> poly) {
    min_y = setting.IMG_LENGHT;
    max_y = 0;
    for (int i = 0; i < poly.size(); i++) {
        if (poly[i].y < min_y) {
            min_y = poly[i].y;
        }
        if (poly[i].y > max_y) {
            max_y = poly[i].y;
        }
    }
}

// check if a cell is in contact with a polygon,
bool Map::contact(std::vector<cv::Point> cell,
                  const std::vector<cv::Point> poly) {
    int max_y, min_y;
    findMaxMinY(max_y, min_y, poly);
    // std::cout<<"poly size : "<<poly.size()<<" max_y : "<<max_y<<" min_y :
    // "<<min_y<<std::endl;
    for (int i = 0; i < cell.size(); i++) {
        cv::Point point = cell[i];
        double x = point.x;
        double y = point.y;

        unsigned short intersections = 0;
        for (int j = 0; j < poly.size(); j++) {
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
            if (y > max_y || y < min_y) {
                continue;
            }


            // where intersect at point x ?
            double y_int = p1.y + (p2.y - p1.y) * (x - p1.x) / (p2.x - p1.x);

            // if y_int is higher than y do intersect
            if (y <= y_int) {
                intersections++;
            }
        }
        /* is odd */
        if (intersections % 2) {
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

std::vector<std::vector<Cell*>> Map::getNearestNeighbors(Cell * &cell){
    return {
        getTopNeighbors(cell),
        getLeftNeighbors(cell),
        getBottomNeighbors(cell),
        getRightNeighbors(cell)
        
    };
}

std::vector<std::vector<Cell*>> Map::getNearestNeighbors(cv::Point cellInPoint){
    
    Cell* cell = getCell(cellInPoint);
    
    return {
        getTopNeighbors(cell),
        getLeftNeighbors(cell),
        getBottomNeighbors(cell),
        getRightNeighbors(cell)
        
    };
}

std::vector<std::vector<Cell*>> Map::getEmptyNearestNeighbors(Cell * &cell){
    
    std::vector<std::vector<Cell*>> cells = getNearestNeighbors(cell);
    std::vector<std::vector<Cell*>> emptyCells;
    
    for(int i=0;i<cells.size();i++){
        std::vector<Cell*> subresult;
        for(int j=0;j<cells[i].size();j++)
            if(cells[i][j]->isEmpty())
                subresult.push_back(cells[i][j]);
        emptyCells.push_back(subresult);
    }
    
    return emptyCells;
}

std::vector<std::vector<cv::Point>> Map::getEmptyNearestNeighborsPoints(Cell * &cell){
    std::vector<std::vector<cv::Point>> points;
    std::vector<std::vector<Cell*>> cells = getEmptyNearestNeighbors(cell);
    
    for(int i=0;i<cells.size();i++){
        std::vector<cv::Point> ps;
        for(int j=0;j<cells[i].size();j++)
            ps.push_back(cells[i][j]->center());
        points.push_back(ps);
    }
    
    
    return points;
}

std::vector<Cell *> Map::getTopNeighbors(Cell* &forCell){
    
    double multiplier = n_multiplier;
    
    double translation_y = multiplier*forCell->height();
    double translation_x = multiplier*forCell->width();
    
    std::vector<Cell*> cells;
    
    cv::Point centerpoint = forCell->center();
    centerpoint.y += translation_y;
    Cell* center = getCell(centerpoint);
    
    if(center != nullptr)
        cells.push_back(center);
    
    cv::Point leftpoint = centerpoint;
    leftpoint.x -= translation_x;
    Cell* left = getCell(leftpoint);
    
    if(left != nullptr)
        cells.push_back(left);
    
    cv::Point rightpoint = centerpoint;
    rightpoint.x += translation_x;
    Cell* right = getCell(rightpoint);
    
    if(right != nullptr)
        cells.push_back(right);
    
    
    return cells;
}
std::vector<Cell *> Map::getLeftNeighbors(Cell* &forCell){
    double translation_y = forCell->height();
    double translation_x = forCell->width();
    
    std::vector<Cell*> cells;
    
    cv::Point centerpoint = forCell->center();
    centerpoint.x -= translation_x;
    Cell* center = getCell(centerpoint);
    
    if(center != nullptr)
        cells.push_back(center);
    
    cv::Point leftpoint = centerpoint;
    leftpoint.y -= translation_y;
    Cell* left = getCell(leftpoint);
    
    if(left != nullptr)
        cells.push_back(left);
    
    cv::Point rightpoint = centerpoint;
    rightpoint.y += translation_y;
    Cell* right = getCell(rightpoint);
    
    if(right != nullptr)
        cells.push_back(right);
    
    
    return cells;
}
std::vector<Cell *> Map::getRightNeighbors(Cell* &forCell){
    double translation_y = forCell->height();
    double translation_x = forCell->width();
    
    std::vector<Cell*> cells;
    
    cv::Point centerpoint = forCell->center();
    centerpoint.x += translation_x;
    Cell* center = getCell(centerpoint);
    
    if(center != nullptr)
        cells.push_back(center);
    
    cv::Point leftpoint = centerpoint;
    leftpoint.y -= translation_y;
    Cell* left = getCell(leftpoint);
    
    if(left != nullptr)
        cells.push_back(left);
    
    cv::Point rightpoint = centerpoint;
    rightpoint.y += translation_y;
    Cell* right = getCell(rightpoint);
    
    if(right != nullptr)
        cells.push_back(right);
    
    
    return cells;
}
std::vector<Cell *> Map::getBottomNeighbors(Cell* &forCell){
    double translation_y = forCell->height();
    double translation_x = forCell->width();
    
    std::vector<Cell*> cells;
    
    cv::Point centerpoint = forCell->center();
    centerpoint.y -= translation_y;
    Cell* center = getCell(centerpoint);
    
    if(center != nullptr)
        cells.push_back(center);
    
    cv::Point leftpoint = centerpoint;
    leftpoint.x -= translation_x;
    Cell* left = getCell(leftpoint);
    
    if(left != nullptr)
        cells.push_back(left);
    
    cv::Point rightpoint = centerpoint;
    rightpoint.x += translation_x;
    Cell* right = getCell(rightpoint);
    
    if(right != nullptr)
        cells.push_back(right);
    
    
    return cells;
}

void Map::save(const std::string &path){
    MapEncoder::encode(this, path);
}

cv::Point Map::getStartPoint(){
    return arena.getCorners()[0];
}

void Map::MapEncoder::encode(Map* map, const std::string &filepath){
    std::cout << "\nencoding map to json file for output path -> " << filepath << std::endl;
    std::cout << "\n";
    
    std::ofstream output;
    output.open(filepath);
    
    output << "{\n";
    
    output << "\"_Intro\":\"Laboratory of Applied Robotics -> Map structure\",\n";
    
    
    
    output << "\"PixelScale\":" << map->robo->map_pixelscale << ",\n";
    output << "\"_ArenaPoints\":\"Arena Values are pixel points of the projected image\",\n";
    
    output << "\"Arena\":[";
    
    output << "{\"Point\":{\"x\":" <<
    std::to_string(map->arena.getTopLeft().x) << ",\"y\":" <<
    std::to_string(map->arena.getTopLeft().y) << "}},";
    
    output << "{\"Point\":{\"x\":" <<
    std::to_string(map->arena.getTopRight().x) << ",\"y\":" <<
    std::to_string(map->arena.getTopRight().y) << "}},";
    
    output << "{\"Point\":{\"x\":" <<
    std::to_string(map->arena.getBottomRight().x) << ",\"y\":" <<
    std::to_string(map->arena.getBottomRight().y) << "}},";
    
    output << "{\"Point\":{\"x\":" <<
    std::to_string(map->arena.getBottomLeft().x) << ",\"y\":" <<
    std::to_string(map->arena.getBottomLeft().y) << "}}";
    
    output << "],\n";
    
    output << "\"_Units\":\"All units are mm values based on pixel_scale\",\n";
    output <<  "\"Obstacles\":[";
    
    bool start = true;
    for(auto &&obst : map->obstacles.get()){
        
        if(!start)
            output << ",";
        
        start = false;
        
        output << "{\"Ostacle\":[";
        
        bool entered = false;
        std::string point_str;
        for(auto &&point : obst->getCorners()){
            entered = true;
            cv::Point truePoint = get_real_coordinates(point, map);
            point_str += "{\"Point\":{\"x\":";
            point_str += std::to_string(truePoint.x);
            point_str += ",\"y\":";
            point_str += std::to_string(truePoint.y);
            point_str += "}},";
        }
        
        if(entered)
            point_str.pop_back();
        
        output << point_str;
        output << "]}";
    }
    
    output << "],\n";
    
    output <<  "\"People\":[";
    
    start = true;
    for(auto &&peop : map->people.people){
        
        if(!start)
            output << ",\n";
        
        start = false;
        
        output << "{\"Victim\":";
        
        cv::Point trueCenter = get_real_coordinates(peop.center, map);
        output << "{\"Center\":{\"x\":";
        output << std::to_string(trueCenter.x);
        output << ",\"y\":";
        output << std::to_string(trueCenter.y);
        output << "},";
        
        output << "\"Radius\":" << peop.radius << ",";
        output << "\"Name\":" << peop.name;
        
        output <<"}}";
        
    }
    
    output << "]\n";
    
    output <<  "}";
    
    output.close();
}

cv::Point Map::MapEncoder::get_real_coordinates(const cv::Point &forPoint, Map* inMap){
    
    cv:Point start = inMap->getStartPoint();
    std::pair<double,double> point = Geometry::convertPixelToMillimeterInMapPlane(forPoint, start,inMap->robo->map_pixelscale);
    
    return cv::Point(point.first,point.second);
}

void Map::scalePixelsForMap(){
    std::vector<cv::Point> corners = arena.getCorners();
    double top_dist = cv::norm(corners[0] - corners[1]);
    double pixel_scale = top_dist / Settings::arena_width;
    std::cout << "Map pixel scale: " << pixel_scale << std::endl;
    //Settings::PIXEL_SCALE = pixel_scale;
}
