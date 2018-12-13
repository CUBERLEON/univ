#include "Block.hpp"

#include "sys/Polygon.hpp"
#include "Renderer.hpp"

Block::Block(bool isBulletProof, bool isPassable, Polygon* shape)
: IPositionable({0, 0}, 0), ICollidable(shape), m_isBulletProof(isBulletProof), m_isPassable(isPassable)
{}

Block::Block(const Block& r)
: Block()
{
    copy(r);
}

Block::Block()
: IPositionable({0, 0}, 0), ICollidable(new Polygon(std::vector< std::pair<float, float> >())), m_isBulletProof(0), m_isPassable(0)
{}

Block::~Block()
{}

void Block::draw(Renderer* renderer) {
    renderer->draw(*m_shape, *this);
}

void Block::copy(const Block& r) {
    m_isBulletProof = r.isBulletProof();
    m_isPassable = r.isPassable();
    m_pos = r.getPos();
    m_rotation = r.getRot();
    m_scale = r.getScale();
    delete m_shape;
    m_shape = new Polygon(*r.getShape());
} 