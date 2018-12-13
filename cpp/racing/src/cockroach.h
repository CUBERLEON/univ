#ifndef COCKROACH_H
#define COCKROACH_H

#include <string>
#include "ISerializable.h"

class Cockroach : public ISerializable
{
public:
	Cockroach(std::string name, float max_speed, float accel, float endurance, float mass, std::string strategy_name = "default_strategy");
	Cockroach(const json& r);
	
	std::string getName() const { return m_name; }
	float getMaxSpeed() const { return m_max_speed; }
	float getAccel() const { return m_accel; }
	float getEndurance() const { return m_endurance; }
	float getMass() const { return m_mass; }
	std::string getStrategyName() const { return m_strategy_name; }
	
	json serialize() const;
protected:
private:
	std::string m_name;
	
	float m_max_speed;
	float m_accel;
	float m_endurance;
	float m_mass;
	
	std::string m_strategy_name;
	
	bool deserialize(const json& r);
};

#endif