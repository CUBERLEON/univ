#include "Input.hpp"

#include "sys/Debug.hpp"
#include <cstring>

Input::Input()
{
    m_keyboard = new int[Keyboard::KEYBOARD_CNT];
    m_mouse = new int[Mouse::MOUSE_CNT];
    reset();
}

Input::Input(const json& r)
: Input()
{
    if (!deserialize(r)) {
        throw std::runtime_error("Couldn't initialize Input from json");
        exit(1);
    }
}

Input::~Input() {
    delete[] m_keyboard;
    delete[] m_mouse;
}

json Input::serialize() const {
    json r;
    r["keyboard"] = json::array();
    for (unsigned int i = 0; i < Keyboard::KEYBOARD_CNT; ++i)
        if (m_keyboard[i])
            r["keyboard"].push_back(i);
    r["mouse"] = json::array();
    for (unsigned int i = 0; i < Mouse::MOUSE_CNT; ++i)
        if (m_mouse[i])
            r["mouse"].push_back(i);
    return r;
}

bool Input::deserialize(const json& r) {
    try {
        reset();
        for (unsigned int i = 0; i < r["keyboard"].size(); ++i)
            m_keyboard[(int)r["keyboard"][i]] = 1;
        for (unsigned int i = 0; i < r["mouse"].size(); ++i)
            m_mouse[(int)r["mouse"][i]] = 1;
    } catch (const std::exception& e) {
        Debug::error(e.what());
        reset();
        return false;
    }
    return true;
}

void Input::reset() {
    memset(m_keyboard, 0, sizeof(int) * Keyboard::KEYBOARD_CNT);
    memset(m_mouse, 0, sizeof(int) * Mouse::MOUSE_CNT);
}