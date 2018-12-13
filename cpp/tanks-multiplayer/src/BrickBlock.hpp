#ifndef BRICK_BLOCK_HPP
#define BRICK_BLOCK_HPP

#include "Block.hpp"

class Renderer;

class BrickBlock : public Block {
public:
    BrickBlock(Polygon* shape);

    int getType() const { return TYPE::BRICK; }

    void draw(Renderer* renderer);
protected:
private:
};

#endif