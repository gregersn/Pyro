#define _USE_MATH_DEFINES
#include <cmath>

#include <pyro/pyrographics.h>
#include <pyro/pyrographics_cairo.h>

#include <iostream>
#include <cstring>

namespace Pyro {
    Graphics::Graphics(unsigned int width, unsigned int height, unsigned int channels) :
    Image() {
        this->_width = width;
        this->_height = height;
        this->channels = channels;
        this->data = (void *)malloc(width * height * sizeof(unsigned char) * this->channels);

        this->smooth();
        this->fill(1.0f, 1.0f, 1.0f, 1.0f);
        this->stroke(0.0f, 0.0f, 0.0f, 1.0f);    
        this->strokeweight(1.0f);
        this->strokecap(ROUND);
        this->background(192);
    }

    Graphics::~Graphics() {
    }

    Graphics *Graphics::create(unsigned int width, unsigned int height, GraphicsMode mode){
       switch(mode) {
            case GraphicsMode::CAIRO:
            default:
                return new GraphicsCairo(width, height, 4);
       }
    }
    Graphics *Graphics::create(unsigned int width, unsigned int height){
        return create(width, height, GraphicsMode::CAIRO);
    }

    void Graphics::point(float x, float y) {
        this->line(x, y, x + 1, y + 1);
    }

    void Graphics::endshape(int close) {
        this->_shape.end(close);
        this->shape(this->_shape, 0, 0);
    }

    void Graphics::background(float r, float g, float b, float a) {
        memset(this->data, 0, this->width() * this->height() * 4);
    }

    void Graphics::image(Image *img, float x, float y) {
        if(this->_image_mode == CENTER) {
            x -= img->width() / 2.0f;
            y -= img->height() / 2.0f;
        }

        this->image_impl(img, x, y);
    };


    void Graphics::triangle(Vector a, Vector b, Vector c) {
        Shape s = Shape();
        s.begin();
        s.vertex(a);
        s.vertex(b);
        s.vertex(c);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }


    void Graphics::triangle(float x0, float y0, float x1, float y1, float x2, float y2) {
        Shape s = Shape();
        s.begin();
        s.vertex(x0, y0);
        s.vertex(x1, y1);
        s.vertex(x2, y2);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::rect(float a, float b, float c, float d) {
        if(this->_rect_mode == CENTER) {
            a -= c / 2.0f;
            b -= d / 2.0f;
        }
        
        Shape s = Shape();
        s.begin();
        s.vertex(a, b);
        s.vertex(a + c, b);
        s.vertex(a + c, b + d);
        s.vertex(a, b + d);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) {
        Shape s = Shape();
        s.begin();
        s.curvevertex(x0, y0);
        s.curvevertex(x1, y1);
        s.curvevertex(x2, y2);
        s.curvevertex(x3, y3);
        s.end(OPEN);
        this->shape(s, 0, 0);
    }


    void Graphics::quad(Vector a, Vector b, Vector c, Vector d) {
        Shape s = Shape();
        s.begin();
        s.vertex(a);
        s.vertex(b);
        s.vertex(c);
        s.vertex(d);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) {
        Shape s = Shape();
        s.begin();
        s.vertex(x0, y0);
        s.vertex(x1, y1);
        s.vertex(x2, y2);
        s.vertex(x3, y3);      
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::arc(float x, float y,
                       float w, float h,
                       float start, float end, int mode) {
        Shape s = Shape();
        s.begin();
        if(end < start) {
            end += TWO_PI;
        }
        int steps = 32;
        float da = (end - start) / (float)steps;
        float a = start;
        for(int i = 0; i < steps + 1; i++) {
            a = start + i * da;
            s.vertex(cos(a) * w / 2.0f, sin(a) * h / 2.0f);
        }
        if(mode == PIE)
            s.vertex(0, 0);
        
        if(mode == PIE || mode == CHORD)
            s.end(CLOSE);
        else
            s.end(OPEN);

        this->shape(s, x, y);
    }

    void Graphics::ellipse(float x, float y, float w, float h, unsigned int segments) {
        Shape s = Shape();
        s.begin();
        float da = M_PI / (segments / 2);
        for(unsigned int i = 0; i < segments; i++) {
            s.vertex(cos(i * da) * w / 2 + x, 
                     sin(i * da) * h / 2 + y);
        }
        s.end(CLOSE);

        this->shape(s, 0, 0);
    }

    void Graphics::stroke(float r, float g, float b, float a) {
        this->stroke_color.set(r, g, b, a);
        this->stroke_enable = true;
    }

    void Graphics::stroke(int r, int g, int b, int a) {
        this->stroke(r / 255.0f,
                     g / 255.0f,
                     b / 255.0f,
                     a / 255.0f);
    }

    void Graphics::stroke(int c) {
        this->stroke(c, c, c, 255);
    }

    void Graphics::stroke(int c, int a) {
        this->stroke(c, c, c, a);
    }

    void Graphics::stroke(int r, int g, int b) {
        this->stroke(r, g, b, 255);
    }

    void Graphics::fill(float r, float g, float b, float a) {
        this->fill_color.set(r, g, b, a);
        this->fill_enable = true;
    }

    void Graphics::fill(int r, int g, int b, int a) {
        this->fill(r / 255.0f,
                     g / 255.0f,
                     b / 255.0f,
                     a / 255.0f);
    }


    void Graphics::fill(int c, int a) {
        this->fill(c, c, c, a);
    }

    void Graphics::fill(int r, int g, int b) {
        this->fill(r, g, b, 255);
    }


    void Graphics::fill(int c) {
        this->fill(c, c, c, 255);
    }

    void Graphics::nostroke() {
        this->stroke_enable = false;
    }

    void Graphics::nofill() {
        this->fill_enable = false;
    }

    void Graphics::smooth() {
        this->_smooth = true;
    }

    void Graphics::nosmooth() {
        this->_smooth = false;
    }

    void Graphics::strokeweight(float w) {
        this->stroke_weight = w;
    }

    void Graphics::strokecap(int cap) {
    }
}