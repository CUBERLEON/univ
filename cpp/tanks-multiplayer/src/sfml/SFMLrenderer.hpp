#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

#include "../Renderer.hpp"
#include <SFML/Graphics.hpp>

class SFMLrenderer : public Renderer {
public:
    SFMLrenderer();
    virtual ~SFMLrenderer();

    virtual void createWindow(int width, int height, std::string title) override;
    virtual void recreateWindow() override;
    virtual void destroyWindow() override;
    virtual void setSettings(short antialiasing, bool apply = false) override;
    
    virtual void requestFocus() override;
    virtual void update() override;
    virtual void input() override;
    
    virtual void clear() override;
    virtual void display() override;
    virtual void draw(IDrawable* r) override;
    
    virtual void draw(const Polygon& p, const IPositionable& t) override;
protected:
private:
    sf::RenderWindow* m_window;
};

#endif