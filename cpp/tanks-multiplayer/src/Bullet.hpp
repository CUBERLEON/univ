#ifndef BULLET_HPP
#define BULLET_HPP

#include "sys/IMovable.hpp"
#include "sys/IDrawable.hpp"
#include "sys/ICollidable.hpp"
#include "sys/ISerializable.hpp"

class Renderer;

class Bullet : public IMovable, public IDrawable, public ICollidable, public ISerializable {
public:
    Bullet(float penetration, Polygon* shape);
    Bullet(const json& r);
    virtual ~Bullet(); 
    
    float getPenetration() const { return m_penetration; }
    
    virtual void draw(Renderer* renderer) override;
    
    virtual json serialize() const override;
    virtual bool deserialize(const json& r) override;
protected:
    float m_penetration;
    
    Bullet();
private:
};

#endif