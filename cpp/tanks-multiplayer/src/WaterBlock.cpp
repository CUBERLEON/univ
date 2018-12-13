#include "WaterBlock.hpp"

#include "Renderer.hpp"

WaterBlock::WaterBlock(Polygon* shape)
: Block(false, false, shape)
{
}

WaterBlock::~WaterBlock() {
    
}

void WaterBlock::draw(Renderer* renderer) {
    renderer->draw(getShape(), getPos(), getRot());
}