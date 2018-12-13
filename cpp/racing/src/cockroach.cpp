#include "cockroach.h"

#include <iostream>

Cockroach::Cockroach(std::string name, float max_speed, float accel, float endurance, float mass, std::string strategy_name)
: m_name(name), m_max_speed(max_speed), m_accel(accel), m_endurance(endurance), m_mass(mass), m_strategy_name(strategy_name)
{}

Cockroach::Cockroach(const json& r)
{
    if (!deserialize(r)) {
        throw std::runtime_error("Cockroach creation failed!");
        exit(1);  
    }
}

json Cockroach::serialize() const {
    json r;
    r["name"] = m_name;
    r["max_speed"] = m_max_speed;
    r["accel"] = m_accel;
    r["endurance"] = m_endurance;
    r["mass"] = m_mass;
    r["strategy_name"] = m_strategy_name;
    return r;
}

bool Cockroach::deserialize(const json& r) {
    try {
        m_name = r["name"];
        m_max_speed = r["max_speed"];
        m_accel = r["accel"];
        m_endurance = r["endurance"];
        m_mass = r["mass"];
        if (r.count("strategy_name"))
            m_strategy_name = r["strategy_name"];
        else
            m_strategy_name = "default_strategy";
    } catch (const std::exception& e) {
        std::cerr << e.what();
        return false;
    }
    return true;
}