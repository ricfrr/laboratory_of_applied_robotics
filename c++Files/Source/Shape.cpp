#include "../Headers/Shape.hpp"
#include "../Headers/Cell.hpp"

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
    
    

    cell_l.push_back(&cell_i);
}

std::vector<Cell*> Shape::getCell(){
    return cell_l;
} 

