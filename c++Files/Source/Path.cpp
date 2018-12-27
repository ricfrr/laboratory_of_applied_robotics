

#include "../Headers/Path.hpp"

using namespace Path2D;

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
    
    PathCoordinates path_coordinates = PathCoordinates(start_point, end_point, maxCurvature);
    setFinder<DubinPathFinder>(path_coordinates);
    
}

Path::Path(Position start_point, Position end_point, double curvature, Map *map_i) {
    setStartPoint(start_point);
    setEndPoint(end_point);
    setMaxCurvature(curvature);
    setMap(map_i);
    
    PathCoordinates path_coordinates = PathCoordinates(start_point, end_point, maxCurvature);
    setFinder<DubinPathFinder>(path_coordinates);
    
    findPath();
};

template <class T>
Path::Path(Position start_point, Position end_point, double curvature, Map *map_i, T* pathFinder, bool complex){
    setStartPoint(start_point);
    setEndPoint(end_point);
    setMaxCurvature(curvature);
    setMap(map_i);
    
    this->finder = new T(start_point,end_point,curvature,map);
    
    if(complex)
        findPath();
    else
        findPathSimple();
}

template <class T>
Path::Path(PathCoordinates coordinates, Map *map, T* pathFinder, bool complex){
    setStartPoint(coordinates.getInitialPosition());
    setEndPoint(coordinates.getFinalPosition());
    setMaxCurvature(coordinates.getMaxCurvature());
    setMap(map);
    
    this->finder = new T(coordinates,map);
    
    if(complex)
        findPath();
    else
        findPathSimple();
}

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
    
    // 123456789
    // !“§$%&/|\
    // ()
    // []
    // {}
    
    int size = lines.size();
    bool orientation_error = false;
    bool coordinate_error = false;
    
    if(size > 0){
        
        
        
        Position end = lines[size-1].getEndPoint();
        end.orientation_locked = end_point.orientation_locked;
        
        //0 and 2PI is the same -> needs to be included - maybe with sin and cos check
        double end_sinResult = sin(end.orientation);
        double endPoint_sinResult = sin(end_point.orientation);
        double end_cosResult = cos(end.orientation);
        double endPoint_cosResult = cos(end_point.orientation);
        
        if(std::round(end_sinResult*1000)/1000  != std::round(endPoint_sinResult*1000)/1000){
            std::cout << "\nWARNING... orientation of endpoint does not match orientation of last line's endpoint" << std::endl;
            std::cout << "endpoint orientation: " << std::round(end_point.orientation*1000)/1000
            << "\nline's endpoint orientation: " << std::round(end.orientation*1000)/1000 << std::endl;
            std::cout << "You can ignore this warning if endpoint orientation is not locked\n" << std::endl;
            
            orientation_error = true;
        }
        if(std::round(end.getCoordinates().x) != end_point.getCoordinates().x){
            std::cout << "WARNING... x coordinate of endpoint does not match x coordinate of last line's endpoint" << std::endl;
            std::cout << "endpoint: " << end_point.getCoordinates().x << "\nline's endpoint: " << std::round(end.getCoordinates().x) << std::endl;
            coordinate_error = true;
        }
        if(std::round(end.getCoordinates().y) != end_point.getCoordinates().y){
            std::cout << "WARNING... y coordinate of endpoint does not match y coordinate of last line's endpoint" << std::endl;
            std::cout << "endpoint: " << end_point.getCoordinates().y << "\nline's endpoint: " << std::round(end.getCoordinates().y) << std::endl;
            coordinate_error = true;
        }
        
        if(orientation_error && end_point.orientation_locked)
            std::cout << "ERROR... lines could not be set -> wrong endpoint orientation\n" << std::endl;
        else if(coordinate_error)
            std::cout << "ERROR... lines could not be set -> wrong endpoint cordinates\n" << std::endl;
        else{
            end_point = end;
            this->lines = lines;
        }
        
        
        
    }
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

    double iterations = 30.0;
    double it = 2.0;
    double initLength = 1000000000;
    std::vector<cv::Point> alt_points;
    
    do{
        getMap()->n_multiplier = it;
        dubin_lines = finder->shortestPath(alt_points);
        
        if(dubin_lines.empty()){
            //std::cout << "will break down path" << std::endl;
            //std::cout << "found " << alt_points.size() << " alternative points" << std::endl;
            
            for(int i=0;i<alt_points.size();i++){
                std::cout << "splitting for point " << i+1 << std::endl;
                Path path = Path(*this);
                path.start_point = start_point;
                path.end_point = end_point;
                path.maxCurvature = maxCurvature;
                path.map = map;
                path.setLines(lines);
                map->n_multiplier = 2.0;
                Path::split<DubinPathFinder>(path, alt_points[i]);
                
                double l = path.getLength();

                if( l > 1 && l < initLength && path.lines.size() > 3){
                    best_lines = path.lines;
                    initLength = l;
                    // the breaks speeds up the process but some solution will get lost
                    if(map->quickCalculation)
                        break;
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

template <class T>
void Path::split(Path &path, cv::Point intermediate){
    
    double orientation_estimation =
    Geometry::angle_rad(path.start_point.getCoordinates(),
                        path.end_point.getCoordinates());
    double orientation_i = orientation_estimation;
    double orientation_e = path.end_point.orientation;
    double max_orientation = orientation_e + 2.0*M_PI;
    Path one,two;
    bool ok = false;
    
    do{
        
        orientation_i = orientation_estimation;
        
        if(!path.end_point.orientation_locked)
            path.end_point.orientation = orientation_e;
        
         //std::cout << "will try with endpoint orientation of " << path.end_point.getOrientation() << std::endl;
        
    while(orientation_i < orientation_estimation + 2.01*M_PI){
        
        Position point = Position(intermediate,orientation_i);
        
        one = Path(PathCoordinates(path.start_point, point, path.maxCurvature), path.map, new T(PathCoordinates(path.start_point, point, path.maxCurvature), path.map),false);
        
        //one = Path(PathCoordinates(path.start_point, point, path.maxCurvature),path.map,false);
        
        two = Path(PathCoordinates(point, path.end_point, path.maxCurvature),path.map, new T(PathCoordinates(point, path.end_point, path.maxCurvature),path.map),false);
        
        if(one.length > 0 && two.length > 0){
            ok = true;
            break;
        }
        
        orientation_i += 0.1;
    }
        orientation_e += M_PI/4.0;
    }while(orientation_e < max_orientation && path.end_point.orientation_locked == false && ok == false);
    
    if(ok){
        path = Path(one,two);
        //std::cout << "succesfully split the path" << std::endl;
    }
    else
        std::cout << "could not split the path" << std::endl;
    
}

void Path::findPathSimple(){
    
    std::vector<Line> dubin_lines = {};
    
    setLength(0);
    
    std::vector<cv::Point> alt_points;
    dubin_lines = finder->shortestPath(alt_points);
    
    if(dubin_lines.empty()){
        //std::cout << "did not find simple path" << std::endl;
        setLength(0);
        return;
    }
    //else
        //std::cout << "found simple path" << std::endl;
    
    
    setLines(dubin_lines);
    if(lines.empty()){
        std::cout << "ERROR" << std::endl;
        setLength(0);
        return;
    }
    
    
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

template <class T>
void Path::setFinder(PathCoordinates pc){
    
    this->finder = new T(pc,map);
}




