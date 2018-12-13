#ifndef TANK_HPP
#define TANK_HPP

#include <string>
#include "sys/IMovable.hpp"
#include "sys/IDrawable.hpp"
#include "sys/ICollidable.hpp"
#include "sys/ISerializable.hpp"

class Renderer;
class Polygon;

class Tank : public IMovable, public IDrawable, public ICollidable, public ISerializable {
public:
    Tank(const std::string& playerName, float enginePower, float maxSpeed, Polygon* shape);
    Tank(const json& r);
    virtual ~Tank();
    
    const std::string& getPlayerName() const { return m_playerName; }
    float getEnginePower() const { return m_enginePower; }
    float getMaxSpeed() const { return m_maxSpeed; }
    int getLevel() const { return m_level; }
    
    virtual void draw(Renderer* renderer) override;
    
    virtual json serialize() const override;
    virtual bool deserialize(const json& r) override;
protected:
    Tank();
private:
    std::string m_playerName;

    float m_enginePower;
    float m_maxSpeed;
    
    float m_health;
    
    int m_level;
    //std::vector<Bonus*> m_bonuses;
};

#endif