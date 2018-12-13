#ifndef TRACK_H
#define TRACK_H

#include <vector>
#include "world.h"

class Track : public ISerializable
{
public:
    Track(float length, float width);
    Track(const json& r);
    
    bool run();
    void reset();
    
    bool isRunFinished() const { return m_run_finished; }
    std::vector<int> getStandings() const { return m_standings; }
    World getWorld() { return m_world; }

    void setCockroaches(const std::vector<Cockroach>& c);
    void setObstacles(const std::vector<Obstacle>& r);
    
    json serialize() const;
    
    static const int RUN_TIME;
protected:
private:
    bool m_run_finished;
    
    std::vector<int> m_standings;
    World m_world;

    bool deserialize(const json& r);
    
    bool standingsCmp(const int& a, const int& b);
};

#endif