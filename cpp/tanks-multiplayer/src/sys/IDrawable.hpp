#ifndef I_DRAWABLE_HPP
#define I_DRAWABLE_HPP

class Renderer;

class IDrawable {
public:
    virtual void draw(Renderer* renderer) = 0;
protected:
    IDrawable() {}
    ~IDrawable() {}
private:
};

#endif