#include "Core.hpp"

#include <chrono>
#include <thread>
#include "sys/Time.hpp"
#include "sys/Utils.hpp"
#include "sys/Debug.hpp"
#include "Tank.hpp"
#include "World.hpp"
#include "Map.hpp"
#include "Input.hpp"
#include "sys/Polygon.hpp"
#ifdef SFML
    #include "sfml/SFMLrenderer.hpp"
#endif

Core::Core()
: m_isRunning(false), m_renderer(nullptr), m_fpsLimit(60), m_lowRateUpdateLimit(30), m_sleepTime(1), m_world(nullptr)
{
#ifdef SFML
    m_renderer = new SFMLrenderer();
#endif
}

Core::~Core() {
    delete m_renderer;
}

Renderer* Core::getRenderer() const {
    if (m_renderer == nullptr)
        Debug::warning("Returning nullptr in Core.getRenderer()");
    return m_renderer;
}

void Core::start() {
    if (m_isRunning)
        return;
    
    try {
        init();
        run();
    } catch (const std::exception& e) {
        Debug::error(e.what());
        stop();
    }
}

void Core::stop() {
    m_isRunning = false;
    
    if (m_renderer->isActive())
        m_renderer->destroyWindow();
}

void Core::setWorld(World* world) {
    m_world = world;
    m_renderer->setDrawTransform(true);
    m_renderer->setDrawTransform(m_world->getMap()->getWidth(), m_world->getMap()->getHeight());    
}

void Core::setWorld(const json& r) {
    if (!m_world)
        m_world = new World(r);
    else
        m_world->deserialize(r);
    setWorld(m_world);
}

void Core::init() {
    if (m_renderer->isActive()) {
        m_renderer->requestFocus();
    }
}

void Core::run() {
    m_isRunning = true;
    
    double prevTime = Time::getTime();

    // double fpsTime = 0;
    // double fpsRefreshTime = 1.0;
    // int frames = 0;
    
    double frameTime = 1.0 / (double)m_fpsLimit;
    double unprocessedFrameTime = 0;
    
    double lowRateUpdateTime = 1.0 / (double)m_lowRateUpdateLimit;
    double unprocessedLowRateUpdateTime = 0;

    while (m_isRunning) {
        double curTime = Time::getTime();
        double passedTime = curTime - prevTime;

        unprocessedLowRateUpdateTime += passedTime;
        unprocessedFrameTime += passedTime;
        // fpsTime += passedTime;

        bool needLowRateUpdate = false;
        double updateTime = 0.;
        if (unprocessedLowRateUpdateTime >= lowRateUpdateTime) {
            updateTime = lowRateUpdateTime * (int)floor(unprocessedLowRateUpdateTime / lowRateUpdateTime);
            unprocessedLowRateUpdateTime -= lowRateUpdateTime;
            needLowRateUpdate = true;
        }
        
        bool needRedraw = false;
        if (unprocessedFrameTime >= frameTime) {
            double tmp = frameTime * (int)floor(unprocessedFrameTime / frameTime);
            unprocessedFrameTime -= tmp;
            needRedraw = true;
        }

        // if (fpsTime >= fpsRefreshTime) {
        //     // Debug::info("%.1f fps", frames / fpsTime);
        //     fpsTime -= fpsRefreshTime;
        //     frames = 0;
        // }
        
        highRateUpdate(passedTime);
        
        if (needLowRateUpdate) {
            if (m_renderer->isActive()) {
                m_renderer->input();
                m_renderer->update();
            }
            
            lowRateUpdate(updateTime);
        }

        if (needRedraw && m_renderer->isActive()) {
            m_renderer->clear();
            
            if (m_world)
                m_renderer->draw(m_world);
            // m_renderer->draw(m_interface);
            
            m_renderer->display();
            
            // frames++;
        }
        else {
            if (m_sleepTime)
                std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepTime));
        }
        
        prevTime = curTime;
    }
    
    m_isRunning = false;
}