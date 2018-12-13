#include "Tank.hpp"

#include "Renderer.hpp"
#include "sys/Polygon.hpp"
#include "sys/Debug.hpp"

Tank::Tank(const std::string& playerName, float enginePower, float maxSpeed, Polygon* shape)
: IMovable({0, 0}, {0, 0}, 0), ICollidable(shape), m_playerName(playerName), m_enginePower(enginePower), m_maxSpeed(maxSpeed), m_health(100), m_level(1)
{}

Tank::Tank(const json& r)
: Tank()
{
    if (!deserialize(r)) {
        throw std::runtime_error("Couldn't initialize Tank from json data");
        exit(1);
    }
}

Tank::Tank()
: IMovable({0, 0}, {0, 0}, 0), ICollidable(new Polygon(std::vector< std::pair<float, float> >())), m_enginePower(0), m_maxSpeed(0), m_health(100), m_level(1)
{}

Tank::~Tank()
{}

void Tank::draw(Renderer* renderer) {
    renderer->draw(*m_shape, *this);
}

json Tank::serialize() const {
    json r;
    r["player_name"] = m_playerName;
    r["engine_power"] = m_enginePower;
    r["max_speed"] = m_maxSpeed;
    r["health"] = m_health;
    r["level"] = m_level;
    r["shape"] = m_shape->serialize();
    r["pos"] = { getPosX(), getPosY() };
    r["rot"] = getRot();
    r["scale"] = { getScaleX(), getScaleY() };
    r["speed"] = { getSpeedX(), getSpeedY() };
    return r;
}

bool Tank::deserialize(const json& r) {
    try {
        m_playerName = r["player_name"];
        m_enginePower = r["engine_power"];
        m_maxSpeed = r["max_speed"];
        m_health = r["health"];
        m_level = r["level"];
        m_shape->deserialize(r["shape"]);
        setPos({(float)r["pos"][0], (float)r["pos"][1]});
        setRot((float)r["rot"]);
        setScale({(float)r["scale"][0], (float)r["scale"][1]});
        setSpeed({(float)r["speed"][0], (float)r["speed"][1]});
    } catch (const std::exception& e) {
        Debug::error(e.what());
        return false;
    }
    return true;
}