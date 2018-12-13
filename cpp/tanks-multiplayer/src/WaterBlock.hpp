#ifndef WATER_BLOCK_HPP
#define WATER_BLOCK_HPP

#include "Block.hpp"

class Renderer;

class WaterBlock : public Block {
public:
    WaterBlock(Polygon* shape);
    ~WaterBlock();

    int getType() const { return TYPE::WATER; }

    void draw(Renderer* renderer);
protected:
private:
};

#endif