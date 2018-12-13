#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "sys/IDrawable.hpp"
#include "sys/IPositionable.hpp"
#include "sys/ICollidable.hpp"

class Polygon;
class Renderer;

class Block : public IDrawable, public IPositionable, public ICollidable {
public:
    // enum TYPE {
    //     BRICK,
    //     WATER,
    //     TYPES_CNT
    // };
    
    Block(bool isBulletProof, bool isPassable, Polygon* shape);
    Block(const Block& r);
    virtual ~Block();
        
    bool isBulletProof() const { return m_isBulletProof; }
    bool isPassable() const { return m_isPassable; }
    
    virtual void draw(Renderer* renderer) override;
    
    // virtual int getType() const = 0;
protected:
    Block();
private:
    bool m_isBulletProof;
    bool m_isPassable;
    
    void copy(const Block& r);
};

#endif