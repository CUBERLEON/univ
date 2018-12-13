#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "cockroach.h"
#include "obstacle.h"
#include "state.h"
#include "ISerializable.h"

class World : public ISerializable
{
public:
    World(float length, float width);
    World(const json& r);

    float getLength() const { return m_length; }
    float getWidth() const { return m_width; }
    std::vector<Cockroach> getCockroaches() const { return m_cockroaches; }
    std::vector<Obstacle> getObstacles() const { return m_obstacles; }
    std::vector<State> getStates() const { return m_states; }
    
    void updateAll(const std::vector<Move>& r);
    void update(const Move& r, const unsigned int i);

    void setCockroaches(const std::vector<Cockroach>& c);
    void setObstacles(const std::vector<Obstacle>& r);

    json serialize() const;
protected:
private:
    float m_length;
    float m_width;
    
    std::vector<Cockroach> m_cockroaches;
    std::vector<Obstacle> m_obstacles;
    std::vector<State> m_states;
    
    bool deserialize(const json& r);
};

#endif