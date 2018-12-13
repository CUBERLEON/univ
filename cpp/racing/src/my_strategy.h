#ifndef MY_STRATEGY_H
#define MY_STRATEGY_H

#include "IStrategy.h"

class MyStrategy: public IStrategy
{
public:
    MyStrategy() : IStrategy(NAME) {}

    void move(const Cockroach& self, const State& state, const World& world, Move& move) {
        move.setSpeed({100, 100});
    }
    
    static const std::string NAME; 
protected:
private:    
};

const std::string MyStrategy::NAME = "my_strategy";

#endif