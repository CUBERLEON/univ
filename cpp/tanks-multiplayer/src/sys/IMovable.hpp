#ifndef I_MOVABLE_HPP
#define I_MOVABLE_HPP

#include "IPositionable.hpp"

class IMovable : public IPositionable {
public:
    const std::pair<float, float>& getSpeed() const { return m_speed; }
    float getSpeedX() const { return m_speed.first; }
    float getSpeedY() const { return m_speed.second; }
    void setSpeed(const std::pair<float, float>& speed) { m_speed = speed; }
    void setSpeedX(float speedX) { m_speed.first = speedX; }
    void setSpeedY(float speedY) { m_speed.second = speedY; }
protected:
    IMovable(std::pair<float, float> speed, const std::pair<float, float> pos, float angle) : IPositionable(pos, angle), m_speed(speed) {}
    ~IMovable() {}
    
    std::pair<float, float> m_speed;
private:
};

#endif