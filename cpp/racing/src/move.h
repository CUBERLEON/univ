#ifndef MOVE_H
#define MOVE_H

#include <utility>

class Move
{
public:
    Move(const std::pair<float,float>& speed = {0, 0}) : m_speed(speed) {}
    
    void setSpeed(const std::pair<float,float>& r) { m_speed = r; }
    void setSpeedX(float x) { m_speed.first = x; }
    void setSpeedY(float y) { m_speed.second = y; }
    std::pair<float,float> getSpeed() const { return m_speed; }
    float getSpeedX() const { return m_speed.first; }
    float getSpeedY() const { return m_speed.second; }
protected:
private:
    std::pair<float,float> m_speed;
};

#endif