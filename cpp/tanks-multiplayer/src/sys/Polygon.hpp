#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include <map>
#include <utility>
#include "ISerializable.hpp"

class Polygon : public ISerializable {
public:
    Polygon(const Polygon& r);
    Polygon(const std::string& fileName);
    Polygon(const std::vector< std::pair<float, float> >& points);
    ~Polygon();
    
    bool isIntersecting(const std::pair<float, float>& a, const std::pair<float, float>& b) const;
    
    const std::vector< std::pair<float, float> >& getPoints() const { return m_points; }
    const std::string& getFileName() const { return m_fileName; }
    
    static void freeLoaded();
    
    virtual json serialize() const override;
    virtual bool deserialize(const json& r) override;
protected:
    std::vector< std::pair<float, float> > m_points;
private:
    static std::map<std::string, Polygon*> m_loaded;
    std::string m_fileName;
    
    void loadFromFile(std::string fileName);
    void copy(const Polygon& r);
};

#endif