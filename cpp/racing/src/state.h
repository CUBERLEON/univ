#ifndef STATE_H
#define STATE_H

#include <utility>
#include "move.h"
#include "ISerializable.h"

class World;
class Cockroach;

class State : public ISerializable
{
public:
    State(const std::pair<float, float>& pos, const std::pair<float, float>& speed);
    State(const json& r);
    
    std::pair<float, float> getPos() const { return m_pos; }
    float getPosX() const { return m_pos.first; }
    float getPosY() const { return m_pos.second; }
    std::pair<float, float> getSpeed() const { return m_speed; }
    float getSpeedX() const { return m_speed.first; }
    float getSpeedY() const { return m_speed.second; }
    
    void update(const Move& m, const World& w, const Cockroach& c);
    
    json serialize() const;
protected:
private: 
    std::pair<float, float> m_pos;
    std::pair<float, float> m_speed;
    
    bool deserialize(const json& r);
};

#endif