#include "pyro/style.h"

namespace Pyro
{
    StyleStack::StyleStack()
    {
        this->current = t_style{};
    }

    void StyleStack::push() { this->stack.push_back(this->current); }
    void StyleStack::pop()
    {
        this->current = this->stack[this->stack.size() - 1];
        this->stack.pop_back();
    }
    int StyleStack::imagemode()
    {
        return this->current.image_mode;
    }

    void StyleStack::imagemode(int mode)
    {
        this->current.image_mode = mode;
    }

    void StyleStack::colormode(int mode)
    {
        this->current.color_mode = mode;
        this->current.stroke.colormode(mode);
        this->current.fill.colormode(mode);
    }

    int StyleStack::rectmode()
    {
        return this->current.rect_mode;
    }

    void StyleStack::rectmode(int mode)
    {
        this->current.rect_mode = mode;
    }

    void StyleStack::ellipsemode(int mode)
    {
        this->current.ellipse_mode = mode;
    }

    float StyleStack::textsize()
    {
        return this->current.text_size;
    }

    void StyleStack::textsize(float size)
    {
        this->current.text_size = size;
    }

    float StyleStack::strokeweight()
    {
        return this->current.stroke_weight;
    }

    float StyleStack::strokeweight(float w)
    {
        this->current.stroke_weight = w;
        return w;
    }

    void StyleStack::nostroke()
    {
        this->current.stroke_enable = false;
    }

    bool StyleStack::stroke_enabled()
    {
        return this->current.stroke_enable;
    }

    void StyleStack::nofill()
    {
        this->current.fill_enable = false;
    }

    bool StyleStack::fill_enabled()
    {
        return this->current.fill_enable;
    }

    Color StyleStack::fill()
    {
        return this->current.fill;
    }

    Color StyleStack::fill(float r, float g, float b, float a)
    {
        this->current.fill.set(r, g, b, a);
        this->current.fill_enable = true;
        return this->current.fill;
    }

    Color StyleStack::stroke()
    {
        return this->current.stroke;
    }

    Color StyleStack::stroke(float r, float g, float b, float a)
    {
        this->current.stroke.set(r, g, b, a);
        this->current.stroke_enable = true;
        return this->current.stroke;
    }

    void StyleStack::strokecap(int cap)
    {
        this->current.stroke_cap = cap;
    }

    int StyleStack::strokecap()
    {
        return this->current.stroke_cap;
    }

    void StyleStack::strokejoin(int join)
    {
        this->current.stroke_join = join;
    }

    int StyleStack::strokejoin()
    {
        return this->current.stroke_join;
    }
}
