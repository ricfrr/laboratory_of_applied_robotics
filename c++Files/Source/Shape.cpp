#include "../Headers/Shape.hpp"
#include "../Headers/Cell.hpp"

using namespace Geometry2D;

Shape::Shape()
{
    // creator
}
Shape::~Shape()
{
    // nada
}

void Shape::setCell(Cell &cell_i){
    
    //check state
    // empty - mixed - full
    Cell* pointer = &cell_i;
    cell_l.push_back(pointer);
}

std::vector<Cell*> Shape::getCell(){
    return cell_l;
} 

