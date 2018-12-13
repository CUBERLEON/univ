#ifndef INPUT_HPP
#define INPUT_HPP

#include "sys/ISerializable.hpp"

class Input : public ISerializable{
public:
    enum Keyboard {
        A = 0,
        W,
        S,
        D,
        KEYBOARD_CNT
    };
    enum Mouse {
        Left = 0,
        Right,
        MOUSE_CNT
    };
    
    Input();
    Input(const json& r);
    virtual ~Input();
    
    int getKeyboardKeyState(unsigned int key) const { return m_keyboard[key]; }
    int getMouseKeyState(unsigned int key) const { return m_mouse[key]; }
    
    json serialize() const override;
    bool deserialize(const json& r) override;
protected:
    int* m_keyboard;
    int* m_mouse;
    
    void reset();
private:
};

#endif