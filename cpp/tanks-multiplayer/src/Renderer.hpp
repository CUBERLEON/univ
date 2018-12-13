#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <utility>
#include <string>

class IDrawable;
class Polygon;
class IPositionable;
class Input;

class Renderer {
public:
    virtual ~Renderer();
    
    bool isActive() const { return m_active; }
    short getAntialiasing() const { return m_antialiasing; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    std::string getTitle() const { return m_title; }
    Input* getInput() const;
    
    virtual void createWindow(int width, int height, std::string title) = 0;
    virtual void recreateWindow() = 0;
    virtual void destroyWindow() = 0;
    virtual void setSettings(short antialiasing, bool apply = false) = 0;
    
    virtual void requestFocus() = 0;
    virtual void update() = 0;
    virtual void input() = 0;
    
    virtual void clear() = 0;
    virtual void display() = 0;
    virtual void draw(IDrawable* r) = 0;
    
    virtual void draw(const Polygon& p, const IPositionable& t) = 0;
    
    void setDrawTransform(bool enabled) { m_activeDT = enabled; }
    bool isActiveDrawTransform() const { return m_activeDT; }
    void setDrawTransform(int width, int height) { m_widthDT = width; m_heightDT = height; }
protected:
    Renderer();
    
    bool m_active;
    
    int m_width;
    int m_height;
    std::string m_title;
    
    short m_antialiasing;
    
    bool m_activeDT;
    int m_widthDT;
    int m_heightDT;
    
    Input* m_input;
private:
};

#endif