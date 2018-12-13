#ifndef ISTRATEGY_H
#define ISTRATEGY_H

#include <string>
#include "cockroach.h"
#include "world.h"
#include "move.h"
#include "state.h"

class IStrategy
{
public:
    IStrategy(const std::string& name) : m_name(name) {}
    virtual ~IStrategy() {}
    
    virtual void move(const Cockroach& self, const State& state, const World& world, Move& move) = 0;   
    std::string getName() const { return m_name; };
protected:
private:
    std::string m_name;  
};

#endif