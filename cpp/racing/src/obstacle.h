#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "ISerializable.h"

class Obstacle : public ISerializable
{
public:
    Obstacle(std::pair<float, float> pos, float radius, float mass);
    Obstacle(const json& r);
    
    std::pair<float, float> getPos() const { return m_pos; }
    float getPosX() const { return m_pos.first; }
    float getPosY() const { return m_pos.second; }
    float getRadius() const { return m_radius; }
    float getMass() const { return m_mass; }
    
    json serialize() const;
protected:
private:
    std::pair<float, float> m_pos;
    float m_radius;
    float m_mass;
    
    bool deserialize(const json& r);
};

#endif