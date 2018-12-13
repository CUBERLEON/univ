#include "state.h"

#include "world.h"
#include "cockroach.h"
#include "utils.h"
#include <algorithm>

State::State(const std::pair<float, float>& pos, const std::pair<float, float>& speed)
: m_pos(pos), m_speed(speed)
{}

State::State(const json& r) {
    if (!deserialize(r)) {
        throw std::runtime_error("State creation failed!");
        exit(1);  
    }
}

void State::update(const Move& m, const World& w, const Cockroach& c) {
    std::pair<float, float> newSpeed = m_speed, newPos = m_pos;
    //--------------------
    
    std::pair<float, float> d = m.getSpeed() - m_speed;
    float tmp = length(d);
    if (tmp > c.getAccel())
        d *= c.getAccel() / tmp;
    newSpeed += d;
    
    for (unsigned i = 0; i < w.getObstacles().size(); ++i) {
        float dist = length(m_pos - w.getObstacles()[i].getPos()); 
        if (dist <= w.getObstacles()[i].getRadius()) {
            pair<float, float> v = (w.getObstacles()[i].getPos() - getPos()) / dist;
            newSpeed += v * (w.getObstacles()[i].getMass() / sqr(dist));
        }
    }
    
    //--------------------    
    float l = length(newSpeed);
    if (l > c.getMaxSpeed())
        newSpeed *= c.getMaxSpeed() / l;
        
    newPos += newSpeed;
    newPos = { std::max(0.f, std::min(w.getLength(), newPos.first)), std::max(0.f, std::min(w.getWidth(), newPos.second)) };
    
    if (newPos.first <= EPS || newPos.first + EPS >= w.getLength())
        newSpeed.first = 0;
    if (newPos.second <= EPS || newPos.second + EPS >= w.getWidth())
        newSpeed.second = 0;
    
    m_speed = newSpeed;
    m_pos = newPos;
}

json State::serialize() const {
    json r;
    r["pos"]["x"] = m_pos.first;
    r["pos"]["y"] = m_pos.second;
    r["speed"]["x"] = m_speed.first;
    r["speed"]["y"] = m_speed.second;
    return r;
}

bool State::deserialize(const json& r) {
    try {
        m_pos.first = r["pos"]["x"];
        m_pos.second = r["pos"]["y"];
        m_speed.first = r["speed"]["x"];
        m_speed.second = r["speed"]["y"];
    } catch (const std::exception& e) {
        std::cerr << e.what();
        return false;
    }
    return true;    
}