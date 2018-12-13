#ifndef DEFAULT_STRATEGY_H
#define DEFAULT_STRATEGY_H

#include "IStrategy.h"

class DefaultStrategy: public IStrategy
{
public:
    DefaultStrategy() : IStrategy(NAME) {}

    void move(const Cockroach& self, const State& state, const World& world, Move& move) {
        move.setSpeed({100, 0});
    }
    
    static const std::string NAME;
protected:
private:    
};

const std::string DefaultStrategy::NAME = "default_strategy";

#endif