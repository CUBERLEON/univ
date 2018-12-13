#include "World.hpp"

#include "Map.hpp"
#include "Tank.hpp"
#include "Bullet.hpp"
#include "Renderer.hpp"
#include "sys/Debug.hpp"

World::World(Map* map)
: m_map(map)//, m_updateTime(1.), m_curTime(0.)
{}

World::World(const json& r)
: m_map(new Map(std::string("default")))
{
    if (!deserialize(r)) {
        throw std::runtime_error("Couldn't initialize World from json data");
        exit(1);
    }
}

World::~World() {
    delete m_map;
    for (unsigned int i = 0; i < m_tanks.size(); ++i)
        delete m_tanks[i];
    for (unsigned int i = 0; i < m_bullets.size(); ++i)
        delete m_bullets[i];
}

Tank* World::getTank(const std::string& playerName) const {
    for (unsigned int i = 0; i < m_tanks.size(); ++i) {
        if (m_tanks[i]->getPlayerName() == playerName)
            return m_tanks[i];
    }
    return nullptr;
}

const std::vector<Tank*>& World::getTanks() const {
    return m_tanks;
}

const std::vector<Bullet*>& World::getBullets() const {
    return m_bullets;
}

Map* World::getMap() const {
    return m_map;
}

void World::addTank(Tank* tank) {
    m_tanks.push_back(tank);
}

void World::addBullet(Bullet* bullet) {
    m_bullets.push_back(bullet);
}

void World::removeTank(const std::string& playerName) {
    for (unsigned int i = 0 ; i < m_tanks.size(); ++i) {
        if (m_tanks[i]->getPlayerName() == playerName)
            m_tanks.erase(m_tanks.begin() + i);
    }
}

void World::update(float time) {
    
}

void World::draw(Renderer* renderer) {
    renderer->draw(m_map);
    for (unsigned int i = 0; i < m_tanks.size(); ++i)
        renderer->draw(m_tanks[i]);
    for (unsigned int i = 0; i < m_bullets.size(); ++i)
        renderer->draw(m_bullets[i]);
}

json World::serialize() const {
    json r;
    r["map"] = m_map->serialize();
    r["tanks"] = json::array();
    for (unsigned int i = 0; i < m_tanks.size(); ++i)
        r["tanks"][i] = m_tanks[i]->serialize();
    r["bullets"] = json::array();
    for (unsigned int i = 0; i < m_bullets.size(); ++i)
        r["bullets"][i] = m_bullets[i]->serialize();
    return r;
}

bool World::deserialize(const json& r) {
    try {
        m_map->deserialize(r["map"]);
        
        for (unsigned int i = 0; i < m_tanks.size(); ++i)
            delete m_tanks[i]; 
        m_tanks.clear();
        for (unsigned int i = 0; i < r["tanks"].size(); ++i)
            m_tanks.push_back(new Tank(r["tanks"][i]));
            
        for (unsigned int i = 0; i < m_bullets.size(); ++i)
            delete m_bullets[i];
        m_bullets.clear();
        for (unsigned int i = 0; i < r["bullets"].size(); ++i)
            m_bullets.push_back(new Bullet(r["bullets"][i]));
            
    } catch (const std::exception& e) {
        Debug::error(e.what());
        return false;
    }
    return true;
}