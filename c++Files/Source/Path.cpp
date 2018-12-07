

#include "../Headers/Path.hpp"

Path::Path() {};

Path::Path(Path path1, Path path2){
   
    //the bigger curve is being reused
    this->maxCurvature = std::max(path1.maxCurvature,path2.maxCurvature);
    //set the starting point to start of path1
    this->start_point = path1.start_point;
    //set the end point to end of path2
    this->end_point = path2.end_point;
    //set the length
    this->length = path1.length + path2.length;
    
    std::vector<Line> line1 = path1.lines;
    std::vector<Line> line2 = path2.lines;
    
    lines.reserve( line1.size() + line2.size() );
    lines.insert( lines.end(), line1.begin(), line1.end() );
    lines.insert( lines.end(), line2.begin(), line2.end() );
    
}

Path::Path(Position start_point, Position end_point, double curvature, Map *map_i) {
    setStartPoint(start_point);
    setEndPoint(end_point);
    setMaxCurvature(curvature);
    setMap(map_i);
    findPath();
};

Path::~Path() {};

void Path::setEndPoint(Position end_point) {
    this->end_point = end_point;
}

Position Path::getEndPoint() {
    return end_point;
}

Position Path::getStartPoint() {
    return start_point;
}

void Path::setMaxCurvature(double curvature) {
    maxCurvature = curvature;
}

double Path::getMaxCurvature() {
    return maxCurvature;
}

void Path::setStartPoint(Position start_point) {
    this->start_point = start_point;
}

void Path::setLines(std::vector<Line> lines) {
    this->lines = lines;
}

std::vector<Line> Path::getLines() {
    return lines;
}


void Path::setLength(double length){
    this->length = length;
};
double Path::getLength(){
    return length;
};

void Path::findPath() {
    
    std::vector<Line> dubin_lines = {};
    std::vector<Line> best_lines;
    
    //some kind of do while (dubin_lines.empty())
    // ...
    PathCoordinates path_coordinates = PathCoordinates(start_point, end_point, maxCurvature);
    
    DubinPath dubin_finder = DubinPath(path_coordinates,getMap());
    
    //find lines - if no lines where found retry with alternative points
    std::vector<cv::Point> alt_points;
    dubin_lines = dubin_finder.dubinShortestPath(alt_points);

    int iterations = 2;
    int it = 0;
    double initLength = 1000000000;
    
    do{
        if(dubin_lines.empty()){
            std::cout << "will break down path" << std::endl;
            std::cout << "found " << alt_points.size() << " alternative points" << std::endl;
            
            for(int i=0;i<alt_points.size();i++){
                std::cout << "splitting for point " << i << std::endl;
                Path path = Path(*this);
                path.start_point = start_point;
                path.end_point = end_point;
                path.maxCurvature = maxCurvature;
                path.map = map;
                path.lines = lines;
                Path::split(path, alt_points[i]);
                
                double l = path.getLength();

                if( l > 1 && l < initLength && path.lines.size() > 3){
                    best_lines = path.lines;
                    initLength = l;
                }
                    
            }
            
            dubin_lines = best_lines;
            
            // here we need to find a new set of points
            // ... for every point get cell and neighboring points
            // ... store them all in an alt_points vector
            std::vector<cv::Point> newset;
            for(int i=0;i<alt_points.size();i++){
                Cell* cell = map->getCell(alt_points[i]);
                std::vector<std::vector<cv::Point>> points = map->getEmptyNearestNeighborsPoints(cell);
                for(int j=0;j<points.size();j++)
                    for(int k=0;k<points[j].size();k++)
                        newset.push_back(points[j][k]);

            }
            
            alt_points = newset;
            
            it++;
            
        }
    }while(dubin_lines.empty() && it < iterations);
    // ...end
    
    if(it >= iterations-1 && dubin_lines.empty())
        std::cout << "could not find a path for " << iterations << " iterations" << std::endl;
    
    setLines(dubin_lines);
    double tmp_length;
    for (int i =0; i<dubin_lines.size(); i++){
        tmp_length += dubin_lines[i].getLength();
    }
    setLength(tmp_length);
}

void Path::split(Path &path, cv::Point intermediate){
    
    double orientation_i = 0;
    double orientation_e = 0;
    Path one,two;
    bool ok = false;
    
    do{
        if(!path.end_point.orientation_locked)
            path.end_point.orientation = orientation_e;
        
        std::cout << "will try with endpoint orientation of " << path.end_point.getOrientation() << std::endl;
        
    while(orientation_i < 2*M_PI){
        
        Position point = Position(intermediate,orientation_i);
        one.start_point = path.start_point;
        one.end_point = point;
        one.maxCurvature = path.maxCurvature;
        one.map = path.map;
        one.lines = path.lines;
        one.findPathSimple();
        
        two.start_point = point;
        two.end_point = path.end_point;
        two.maxCurvature = path.maxCurvature;
        two.map = path.map;
        two.lines = path.lines;
        two.findPathSimple();
        
        if(one.length > 0 && two.length > 0){
            ok = true;
            break;
        }
        
        orientation_i += 0.1;
    }
        orientation_e += 1;
    }while(orientation_e < 2*M_PI && path.end_point.orientation_locked == false && ok == false);
    
    
    if(ok)
        path = Path(one,two);
    else
        std::cout << "could not split the path" << std::endl;
    
}

void Path::findPathSimple(){
    
    std::vector<Line> dubin_lines = {};
    
    PathCoordinates path_coordinates = PathCoordinates(start_point, end_point, maxCurvature);
    DubinPath dubin_finder = DubinPath(path_coordinates,getMap());
    
    std::vector<cv::Point> alt_points;
    dubin_lines = dubin_finder.dubinShortestPath(alt_points);
    
    setLines(dubin_lines);
    double tmp_length;
    for (int i =0; i<dubin_lines.size(); i++){
        tmp_length += dubin_lines[i].getLength();
    }
    setLength(tmp_length);
}

void Path::setMap(Map* map_i) {
    map = map_i;
}

Map* Path::getMap() {
    return map;
}



