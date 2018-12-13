#include "track.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include "default_strategy.h"
#include "my_strategy.h"
#include "utils.h"

const int Track::RUN_TIME = 500;

Track::Track(float length, float width)
: m_run_finished(false), m_world(length, width)
{}

Track::Track(const json& r)
: m_world(r["world"])
{
    if (!deserialize(r)) {
        throw std::runtime_error("Track creation failed!");
        exit(1);  
    }
}

bool Track::run() {
    m_run_finished = false;
    
    if (m_world.getCockroaches().empty()) {
        throw std::invalid_argument("List of cockroaches cannot be empty!");
        return false;
    }
    
    std::vector<IStrategy*> strategies;
    for (unsigned int i = 0; i < m_world.getCockroaches().size(); ++i) {
        if (m_world.getCockroaches()[i].getStrategyName() == MyStrategy::NAME)
            strategies.push_back((IStrategy*)new MyStrategy());
        else
            strategies.push_back((IStrategy*)new DefaultStrategy());
    }
    
    std::vector< std::vector<State> > allStates;
    std::vector<int> finished(m_world.getCockroaches().size(), 0);
    std::vector<int> standings;
        
    allStates.push_back(m_world.getStates());
    for (unsigned int i = 1; i < RUN_TIME && standings.size() != strategies.size(); ++i) {
        std::vector<Move> moves(strategies.size());
        for (unsigned int j = 0; j < strategies.size(); ++j)
            if (!finished[j])
                strategies[j]->move(m_world.getCockroaches()[j], m_world.getStates()[j], m_world, moves[j]);
        for (unsigned int j = 0; j < strategies.size(); ++j)
            if (!finished[j])
                m_world.update(moves[j], j);
        
        std::vector<State> s = m_world.getStates();
        allStates.push_back(s);
    
        for (unsigned int j = 0; j < s.size(); ++j)
            if (!finished[j] && s[j].getPos().first + EPS >= m_world.getLength()) {
                finished[j] = true;
                standings.push_back(j);
            }
    }
    
    int finishedCnt = standings.size();
    for (unsigned int i = 0; i < m_world.getCockroaches().size(); ++i)
        if (!finished[i])
            standings.push_back(i);
    std::sort(standings.begin() + finishedCnt, standings.end(), std::bind(&Track::standingsCmp, this, std::placeholders::_1, std::placeholders::_2));
    m_standings = standings;

    for (unsigned int i = 0; i < m_world.getCockroaches().size(); ++i)
        delete strategies[i];
    
    json data;
    for (unsigned int i = 0; i < allStates.size(); ++i)
        for (unsigned int j = 0; j < allStates[i].size(); ++j)
            data["states"][i][j] = allStates[i][j].serialize();
    data["world"] = m_world.serialize();
    
    std::fstream out;
    std::string fileName = "run_data.dat"; 
    out.open(fileName, std::fstream::out);
    out << data.dump();
    out.close();
    
    #ifdef _WIN32
        system((".\\view.exe " + fileName).c_str());
    #else
        system(("./view " + fileName).c_str());
    #endif
    
    return m_run_finished = true;
}

void Track::reset() {
    m_run_finished = false;
    m_standings.clear();
}

void Track::setCockroaches(const std::vector<Cockroach>& c) {
    m_world.setCockroaches(c);
}

void Track::setObstacles(const std::vector<Obstacle>& r) {
    m_world.setObstacles(r);
}

json Track::serialize() const {
    json r;
    r["run_finished"] = m_run_finished;
    r["world"] = m_world.serialize();
    if (m_run_finished)
        r["standings"] = m_standings;    
    return r;
}

bool Track::deserialize(const json& r) {
    try {
        m_run_finished = r["run_finished"];
        m_world = World(r["world"]);
        if (r["standings"].size() != m_world.getCockroaches().size())
            reset();
        else {
            m_standings.clear();
            for (unsigned int i = 0; i < r["standings"].size(); ++i)
                m_standings.push_back(r["standings"][i]);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what();
        return false;
    }
    return true;
}

bool Track::standingsCmp(const int& a, const int& b) {
    return m_world.getStates()[a].getPos().first < m_world.getStates()[b].getPos().first;
}