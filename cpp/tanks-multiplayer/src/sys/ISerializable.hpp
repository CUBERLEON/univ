#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <string>

#include <json/json.hpp>
using json=nlohmann::json;

class ISerializable
{
public:
    virtual ~ISerializable() {}
    virtual json serialize() const = 0;
protected:
    virtual bool deserialize(const json& r) = 0;
private:
};

#endif