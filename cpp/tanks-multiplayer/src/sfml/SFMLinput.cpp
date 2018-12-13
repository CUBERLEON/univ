#include "SFMLinput.hpp"

#include "../sys/Debug.hpp"
#include <SFML/Window.hpp>

SFMLinput::SFMLinput()
{}

SFMLinput::~SFMLinput()
{}

void SFMLinput::update(const sf::RenderWindow* window) {
    if (!window->hasFocus())
        return;
    
    for (unsigned int i = 0; i < Keyboard::KEYBOARD_CNT; ++i) {
        m_keyboard[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)toSFMLKeyboardKey(i));
        // if (m_keyboard[i])
        //     Debug::info("keyboard %d", i);
    }
    for (unsigned int i = 0; i < Mouse::MOUSE_CNT; ++i) {
        m_mouse[i] = sf::Mouse::isButtonPressed((sf::Mouse::Button)toSFMLMouseButton(i));
        // if (m_mouse[i])
        //     Debug::info("mouse %d", i);
    }
}

int SFMLinput::toSFMLKeyboardKey(int key) {
    int v = -1;
    switch(key) {
        case Keyboard::A: v = sf::Keyboard::A; break;
        case Keyboard::W: v = sf::Keyboard::W; break;
        case Keyboard::S: v = sf::Keyboard::S; break;
        case Keyboard::D: v = sf::Keyboard::D; break;
    }
    return v;
}

int SFMLinput::toSFMLMouseButton(int key) {
    int v = -1;
    switch(key) {
        case Mouse::Left: v = sf::Mouse::Left; break;
        case Mouse::Right: v = sf::Mouse::Right; break;
    }
    return v;
}