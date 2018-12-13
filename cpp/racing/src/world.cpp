#include "world.h"

World::World(float length, float width)
: m_length(length), m_width(width)
{}

World::World(const json& r)
{
    if (!deserialize(r)) {
        throw std::runtime_error("World creation failed!");
        exit(1);  
    }
}

void World::updateAll(const std::vector<Move>& r) {
    if (r.size() != m_states.size())
        throw std::invalid_argument("Number of Moves and States doesn't match!");
    else
        for (unsigned int i = 0; i < r.size(); ++i)
            m_states[i].update(r[i], *this, m_cockroaches[i]);
}

void World::update(const Move& r, const unsigned int i) {
    if (i >= m_states.size())
        throw std::invalid_argument("Index out of range (World::update)!");
    else
        m_states[i].update(r, *this, m_cockroaches[i]);
}

void World::setCockroaches(const std::vector<Cockroach>& c) {
    m_cockroaches = c;
    m_states.clear();
    for (unsigned int i = 0; i < c.size(); ++i)
        m_states.push_back(State({0.f, (float)(i+1)*m_width/(c.size()+1)}, {0.f, 0.f}));
}

void World::setObstacles(const std::vector<Obstacle>& r) {
    m_obstacles = r;
}

json World::serialize() const {
    json r;
    r["length"] = m_length;
    r["width"] = m_width;
    r["cockroaches"] = json::array();
    r["obstacles"] = json::array();
    r["states"] = json::array();
    for (unsigned int i = 0; i < m_cockroaches.size(); ++i)
        r["cockroaches"][i] = m_cockroaches[i].serialize();
    for (unsigned int i = 0; i < m_obstacles.size(); ++i)
        r["obstacles"][i] = m_obstacles[i].serialize();  
    for (unsigned int i = 0; i < m_states.size(); ++i)
        r["states"][i] = m_states[i].serialize();  
    return r;
}

bool World::deserialize(const json& r) {
    try {
        m_length = r["length"];
        m_width = r["width"];
        m_cockroaches.clear();
        for (unsigned int i = 0; r.count("cockroaches") && i < r["cockroaches"].size(); ++i)
            m_cockroaches.push_back(Cockroach(r["cockroaches"][i]));
        m_obstacles.clear();
        for (unsigned int i = 0; r.count("obstacles") && i < r["obstacles"].size(); ++i)
            m_obstacles.push_back(Obstacle(r["obstacles"][i]));
        m_states.clear();
        for (unsigned int i = 0; r.count("states") && i < r["states"].size(); ++i)
            m_states.push_back(State(r["states"][i]));
   } catch (const std::exception& e) {
        std::cerr << e.what();
        return false;
    }
    return true;
}