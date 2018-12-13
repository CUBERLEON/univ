#include "BrickBlock.hpp"

#include "Renderer.hpp"

BrickBlock::BrickBlock(Polygon* shape)
: Block(true, false, shape)
{
}

void BrickBlock::draw(Renderer* renderer) {
    renderer->draw(getShape(), getPos(), getRot());
}