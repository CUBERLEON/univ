#ifndef I_COLLIDABLE_HPP
#define I_COLLIDABLE_HPP

#include "Polygon.hpp"

class ICollidable {
public:
    Polygon* getShape() const { return m_shape; }
    void setShape(Polygon* shape) { m_shape = shape; }
protected:
    ICollidable(Polygon* shape) : m_shape(shape) {}
    ~ICollidable() { delete m_shape; }
    
    Polygon* m_shape;
private:
};

#endif