#define _USE_MATH_DEFINES
#include <math.h>

#include <pyrographics.h>

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <random>
#include <cstring>

namespace Pyro {
    Graphics::Graphics(unsigned int width, unsigned int height, unsigned int bpp) :
    Image() {
        this->_width = width;
        this->_height = height;
        this->bpp = bpp;
        this->data = (void *)malloc(width * height * sizeof(unsigned char) * this->bpp);
        this->surface = cairo_image_surface_create_for_data((unsigned char *)this->data,
                                                            CAIRO_FORMAT_ARGB32,
                                                            this->width(), this->height(),
                                                            this->width() * this->bpp);
        this->cr = cairo_create(this->surface);

        this->smooth();
        this->fill(1.0f, 1.0f, 1.0f, 1.0f);
        this->stroke(0.0f, 0.0f, 0.0f, 1.0f);    
        this->strokeweight(1.0f);
        this->strokecap(ROUND);
        this->background(192);
    }

    Graphics::~Graphics() {
        cairo_destroy(this->cr);
        cairo_surface_destroy(this->surface);
    }

    Graphics *Graphics::create(unsigned int width, unsigned int height){
        Graphics *pg = new Graphics(width, height, 4);        
        return pg;
    }

    int Graphics::random(int range) {
        return random(0, range);
    }

    int Graphics::random(int low, int high) {
        assert(high > low);
        return (rand()%(high - low)) + low;
    }
    
    unsigned int Graphics::random(unsigned int range) {
        return random(0u, range);
    }

    unsigned int Graphics::random(unsigned int low, unsigned int high) {
        assert(high > low);
        return (rand()%(high - low)) + low;
    }
    
    float Graphics::random(float range) {
        return random(0.0f, range);
    }

    float Graphics::random(float low, float high) {
        assert(high > low);
        return ((rand() / (float)RAND_MAX) * (high - low)) + low;
    }

    void Graphics::image(Image *img, float x, float y) {
        cairo_surface_t *src = cairo_image_surface_create_for_data(
            (unsigned char *)img->get_pre_multiplied_data(), CAIRO_FORMAT_ARGB32,
            img->width(), img->height(), img->width() * 4);
        cairo_set_source_surface(this->cr, src, x, y);
        cairo_paint(this->cr);
        cairo_surface_destroy(src);
    
    }
    
    void Graphics::shape(Shape s, float x, float y) {
        cairo_new_path(this->cr);

        cairo_save(this->cr);
        cairo_set_line_width(this->cr, this->stroke_weight);

        cairo_translate(this->cr, x, y);

        for(size_t i = 0; i < s.getpoints().size(); i++) {
            if(i == 0) {
                cairo_move_to(this->cr, s.getpoints()[i].x(), s.getpoints()[i].y());
            } else {
                cairo_line_to(this->cr, s.getpoints()[i].x(), s.getpoints()[i].y());
            }
        }
        if(s.close == CLOSE) {
            cairo_close_path(this->cr);
        }

        if(this->fill_enable) {
            cairo_set_source_rgba(this->cr, this->fill_color.r,
                                            this->fill_color.g,
                                            this->fill_color.b,
                                            this->fill_color.a);
            cairo_fill_preserve(this->cr);
        }

        if(this->stroke_enable) {
            cairo_set_source_rgba(this->cr, this->stroke_color.r,
                                            this->stroke_color.g,
                                            this->stroke_color.b,
                                            this->stroke_color.a);
            cairo_stroke(this->cr);
        }

        cairo_restore(this->cr);
    }

    void Graphics::point(float x, float y) {
        this->line(x, y, x + 1, y + 1);
    }

    void Graphics::translate(float x, float y) {
        cairo_translate(this->cr, x, y);
    }

    void Graphics::rotate(float a) {
        cairo_rotate(this->cr, a);
    }

    void Graphics::pushmatrix() {
        cairo_save(this->cr);
    }

    void Graphics::popmatrix() {
        cairo_restore(this->cr);
    }

    void Graphics::endshape(int close) {
        this->_shape.end(close);
        this->shape(this->_shape, 0, 0);
    }
    void Graphics::line(float x0, float y0, float x1, float y1) {
        cairo_new_path(this->cr);
        cairo_move_to(this->cr, x0, y0);
        cairo_line_to(this->cr, x1, y1);
        cairo_set_line_width(this->cr, this->stroke_weight);
        cairo_set_source_rgba(this->cr,
            this->stroke_color.r,
            this->stroke_color.g,
            this->stroke_color.b,
            this->stroke_color.a);
        cairo_stroke(this->cr);
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
        Shape s = Shape();
        s.begin();
        s.vertex(a, b);
        s.vertex(a + c, b);
        s.vertex(a + c, b + d);
        s.vertex(a, b + d);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) {
        Shape s  = Shape();
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

    void Graphics::ellipse(float x, float y, float w, float h) {
        Shape s = Shape();
        s.begin();
        float da = M_PI / 16;
        for(int i = 0; i < 32; i++) {
            s.vertex(cos(i * da) * w / 2, sin(i * da) * h / 2);
        }
        s.end(CLOSE);

        this->shape(s, x, y);
        return
        cairo_set_source_rgba(this->cr, 0.0, 0.0, 1.0, 1.0);
        cairo_new_path(this->cr);
        
        cairo_arc(this->cr, x, y, w / 2, 0, 2  * M_PI);
        
        cairo_set_line_width(this->cr, 20.0);
        cairo_stroke(this->cr);
    }

    void Graphics::background(float r, float g, float b, float a) {
        memset(this->data, 0, this->width() * this->height() * 4);
        cairo_set_source_rgba(this->cr, r, g, b, a);
        cairo_paint(this->cr);
    }

    void Graphics::stroke(float r, float g, float b, float a) {
        this->stroke_color.r = r;
        this->stroke_color.g = g;
        this->stroke_color.b = b;
        this->stroke_color.a = a;
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
        this->fill_color.r = r;
        this->fill_color.g = g;
        this->fill_color.b = b;
        this->fill_color.a = a;
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
        cairo_set_antialias(this->cr, CAIRO_ANTIALIAS_DEFAULT);
    }

    void Graphics::nosmooth() {
        this->_smooth = false;
        cairo_set_antialias(this->cr, CAIRO_ANTIALIAS_NONE);
    }

    void Graphics::strokeweight(float w) {
        this->stroke_weight = w;
    }

    void Graphics::strokecap(int cap) {
        if(cap == ROUND) {
            cairo_set_line_cap(this->cr, CAIRO_LINE_CAP_ROUND);
        }

        if(cap == SQUARE) {
            cairo_set_line_cap(this->cr, CAIRO_LINE_CAP_BUTT);
        }

        if(cap == PROJECT) {
            cairo_set_line_cap(this->cr, CAIRO_LINE_CAP_SQUARE);
        }
    }

    float Graphics::radians(float degree) { 
        return degree * M_PI / 180.0; 
    }

}