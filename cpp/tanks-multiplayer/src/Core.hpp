#ifndef CORE_HPP
#define CORE_HPP

#include <json/json.hpp>
using json=nlohmann::json;

class Renderer;
class World;

class Core {
public:
    virtual ~Core();
    
    bool isRunning() const { return m_isRunning; }
    Renderer* getRenderer() const;
    
    void start();
    void stop();
protected:
    Core();
    
    bool m_isRunning;

    Renderer* m_renderer;
    int m_fpsLimit;
    
    int m_lowRateUpdateLimit;
    int m_sleepTime;
    
    World* m_world;
    // Interface* m_interface;
    
    virtual void lowRateUpdate(float updateTime) = 0;
    virtual void highRateUpdate(float updateTime) = 0;
    
    void setWorld(World* world);
    void setWorld(const json& r);
private:
    void init();
    void run();
};

#endif