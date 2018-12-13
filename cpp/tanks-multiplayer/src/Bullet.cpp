#include "Bullet.hpp"

#include "Renderer.hpp"
#include "sys/Debug.hpp"

Bullet::Bullet(float penetration, Polygon* shape)
: IMovable({0, 0}, {0, 0}, 0), ICollidable(shape), m_penetration(penetration)
{}

Bullet::Bullet(const json& r)
: Bullet()
{
    if (!deserialize(r)) {
        throw std::runtime_error("Couldn't initialize Bullet from json data");
        exit(1);
    }
}

Bullet::Bullet()
: IMovable({0, 0}, {0, 0}, 0), ICollidable(new Polygon(std::vector< std::pair<float, float> >())), m_penetration(0)
{}

Bullet::~Bullet()
{}

void Bullet::draw(Renderer* renderer) {
    renderer->draw(*m_shape, *this);
}

json Bullet::serialize() const {
    json r;
    r["penetration"] = m_penetration;
    r["shape"] = m_shape->serialize();
    r["pos"] = { getPosX(), getPosY() };
    r["rot"] = getRot();
    r["scale"] = { getScaleX(), getScaleY() };
    r["speed"] = { getSpeedX(), getSpeedY() };
    return r;
}

bool Bullet::deserialize(const json& r) {
    try {
        m_penetration = r["penetration"];
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