#include <pyrographics_cairo.h>

namespace Pyro {
    GraphicsCairo::GraphicsCairo(unsigned int width, unsigned int height, unsigned int bpp) : Graphics(width, height, bpp) {
        this->surface = cairo_image_surface_create_for_data((unsigned char *)this->data,
                                                            CAIRO_FORMAT_ARGB32,
                                                            this->width(), this->height(),
                                                            this->width() * this->bpp);
        this->cr = cairo_create(this->surface);


    }

    GraphicsCairo::~GraphicsCairo() {
        cairo_destroy(this->cr);
        cairo_surface_destroy(this->surface);

    }

    void GraphicsCairo::blendmode(BlendMode mode) {
        switch(mode) {
            case BlendMode::ADD:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_ADD);
                break;
            case BlendMode::BLEND:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_OVER);
                break;
            case BlendMode::DARKEST:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_DARKEN);
                break;
            case BlendMode::DIFFERENCE:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_DIFFERENCE);
                break;
            case BlendMode::EXCLUSION:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_EXCLUSION);
                break;
            case BlendMode::LIGHTEST:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_LIGHTEN);
                break;
            case BlendMode::MULTIPLY:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_MULTIPLY);
                break;
            case BlendMode::REPLACE:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_SOURCE);
                break;
            case BlendMode::SCREEN:
                cairo_set_operator(this->cr, CAIRO_OPERATOR_SCREEN);
                break;
        }
    }

    void GraphicsCairo::shape(Shape s, float x, float y) {
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

    void GraphicsCairo::image(Image *img, float x, float y) {
        cairo_surface_t *src = cairo_image_surface_create_for_data(
            (unsigned char *)img->get_pre_multiplied_data(), CAIRO_FORMAT_ARGB32,
            img->width(), img->height(), img->width() * 4);
        cairo_set_source_surface(this->cr, src, x, y);
        cairo_paint(this->cr);
        cairo_surface_destroy(src);
    
    }
    void GraphicsCairo::translate(float x, float y) {
        cairo_translate(this->cr, x, y);
    }

    void GraphicsCairo::rotate(float a) {
        cairo_rotate(this->cr, a);
    }

    void GraphicsCairo::pushmatrix() {
        cairo_save(this->cr);
    }

    void GraphicsCairo::popmatrix() {
        cairo_restore(this->cr);
    }

    void GraphicsCairo::line(float x0, float y0, float x1, float y1) {
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

    void GraphicsCairo::ellipse(float x, float y, float w, float h) {
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

    void GraphicsCairo::background(float r, float g, float b, float a) {
        Graphics::background(r, g, b, a); //memset(this->data, 0, this->width() * this->height() * 4);
        cairo_set_source_rgba(this->cr, r, g, b, a);
        cairo_paint(this->cr);
    }

    void GraphicsCairo::smooth() {
        this->_smooth = true;
        cairo_set_antialias(this->cr, CAIRO_ANTIALIAS_DEFAULT);
    }

    void GraphicsCairo::nosmooth() {
        this->_smooth = false;
        cairo_set_antialias(this->cr, CAIRO_ANTIALIAS_NONE);
    }

    void GraphicsCairo::strokecap(int cap) {
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
}