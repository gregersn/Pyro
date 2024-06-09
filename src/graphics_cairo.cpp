#include "pyro/graphics_cairo.h"
#include "pyro/font_impl.h"
#include "pyro/utils.h"
#include <cairo-svg.h>
#include <cairo-pdf.h>

namespace Pyro
{
    GraphicsCairo::GraphicsCairo(unsigned int width, unsigned int height, GraphicsMode mode, std::filesystem::path filename) : Graphics(width, height, filename)
    {
        this->mode = mode;
    }

    void GraphicsCairo::init()
    {
        Image::init();
        switch (this->mode)
        {
        case SVG:
            this->surface = cairo_svg_surface_create(filename.c_str(), _width, _height);
            break;
        case PDF:
            this->surface = cairo_pdf_surface_create(filename.c_str(), _width, _height);
            break;
        default:
            this->surface = cairo_image_surface_create_for_data(this->load_bytes(),
                                                                CAIRO_FORMAT_ARGB32,
                                                                this->_pixelwidth, this->_pixelheight,
                                                                this->_pixelwidth * 4);
            break;
        }
        this->cr = cairo_create(this->surface);
        Graphics::init();
    }

    GraphicsCairo::~GraphicsCairo()
    {
        if (this->font != nullptr)
        {
            cairo_font_face_destroy(this->font);
            this->font = nullptr;
        }
        cairo_destroy(this->cr);
        cairo_surface_destroy(this->surface);
    }

    void GraphicsCairo::blendmode(int mode)
    {
        switch (mode)
        {
        case Pyro::REPLACE:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_SOURCE);
            break;
        case Pyro::BLEND:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_OVER);
            break;
        case Pyro::ADD:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_ADD);
            break;
        // case Pyro::SUBTRACT:
        case Pyro::LIGHTEST:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_LIGHTEN);
            break;
        case Pyro::DARKEST:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_DARKEN);
            break;
        case Pyro::DIFFERENCE:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_DIFFERENCE);
            break;
        case Pyro::EXCLUSION:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_EXCLUSION);
            break;
        case Pyro::MULTIPLY:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_MULTIPLY);
            break;
        case Pyro::SCREEN:
            cairo_set_operator(this->cr, CAIRO_OPERATOR_SCREEN);
            break;
        // case Pyro::OVERLAY:
        // case Pyro::HARD_LIGHT:
        // case Pyro::SOFT_LIGHT:
        // case Pyro::DODGE:
        // case Pyro::BURN:
        default:
            throw std::logic_error("Blend mode not implemented.");
            break;
        }
    }

    void GraphicsCairo::shape(Shape s, float x, float y, Unit unit)
    {
        // TODO: Use unit
        cairo_save(this->cr);
        cairo_new_path(this->cr);

        cairo_set_line_width(this->cr, this->stroke_weight);

        cairo_translate(this->cr, x, y);

        for (size_t contour_index{0}; contour_index < s.getpoints().size(); contour_index++)
        {
            std::vector<Pyro::Vector> contour_points{s.getpoints()[contour_index]};

            for (size_t i{0}; i < contour_points.size(); i++)
            {
                if (i == 0)
                {
                    cairo_move_to(this->cr, contour_points[i].x, contour_points[i].y);
                }
                else
                {
                    cairo_line_to(this->cr, contour_points[i].x, contour_points[i].y);
                }
            }
            if (s.close == CLOSE)
            {
                cairo_close_path(this->cr);
            }
        }
        if (this->fill_enable)
        {
            cairo_set_source_rgba(this->cr, this->fill_color.r(),
                                  this->fill_color.g(),
                                  this->fill_color.b(),
                                  this->fill_color.a());
            cairo_fill_preserve(this->cr);
        }

        if (this->stroke_enable)
        {
            cairo_set_source_rgba(this->cr, this->stroke_color.r(),
                                  this->stroke_color.g(),
                                  this->stroke_color.b(),
                                  this->stroke_color.a());
            cairo_stroke(this->cr);
        }

        cairo_restore(this->cr);
    }

    void GraphicsCairo::image_impl(Image *img, float x, float y)
    {
        cairo_surface_t *src{nullptr};
        if (img->channels() == 4)
        {
            src = cairo_image_surface_create_for_data(
                static_cast<unsigned char *>(img->get_pre_multiplied_data()), CAIRO_FORMAT_ARGB32,
                img->width(), img->height(), img->width() * 4);
        }
        if (img->channels() == 3)
        {
            src = cairo_image_surface_create_for_data(
                static_cast<unsigned char *>(img->convert(ARGB)->get_data()), CAIRO_FORMAT_RGB24,
                img->width(), img->height(), img->width() * 4);
        }
        if (img->channels() == 1)
        {
            src = cairo_image_surface_create_for_data(
                static_cast<unsigned char *>(img->convert(ARGB)->get_data()), CAIRO_FORMAT_RGB24,
                img->width(), img->height(), img->width() * 4);
        }

        if (src != nullptr)
        {
            cairo_set_source_surface(this->cr, src, x, y);
            cairo_paint(this->cr);
            cairo_surface_destroy(src);
        }
    }
    void GraphicsCairo::translate(float x, float y, Unit unit)
    {
        if (unit == Unit::CURRENT)
            unit = this->unit;

        x = unit2pixels(x, unit, this->get_dpi());
        y = unit2pixels(y, unit, this->get_dpi());

        cairo_translate(this->cr, x, y);
    }

    void GraphicsCairo::rotate(float a)
    {
        cairo_rotate(this->cr, a);
    }

    void GraphicsCairo::scale(float sx, float sy)
    {
        cairo_scale(this->cr, sx, sy);
    }

    void GraphicsCairo::pushmatrix()
    {
        cairo_save(this->cr);
    }

    void GraphicsCairo::popmatrix()
    {
        cairo_restore(this->cr);
    }

    void GraphicsCairo::line(float x0, float y0, float x1, float y1, Unit unit)
    {
        if (this->stroke_enable)
        {
            if (unit == Unit::CURRENT)
                unit = this->unit;
            x0 = unit2pixels(x0, unit, this->get_dpi());
            y0 = unit2pixels(y0, unit, this->get_dpi());

            x1 = unit2pixels(x1, unit, this->get_dpi());
            y1 = unit2pixels(y1, unit, this->get_dpi());

            cairo_new_path(this->cr);
            cairo_move_to(this->cr, x0, y0);
            cairo_line_to(this->cr, x1, y1);
            cairo_set_line_width(this->cr, this->stroke_weight);
            cairo_set_source_rgba(this->cr,
                                  this->stroke_color.r(),
                                  this->stroke_color.g(),
                                  this->stroke_color.b(),
                                  this->stroke_color.a());
            cairo_stroke(this->cr);
        }
    }

    void GraphicsCairo::ellipse(float x, float y, float w, float h, unsigned int segments)
    {
        Shape s{Shape()};
        s.begin();
        double da{M_PI / (segments / 2)};
        for (unsigned int i{0}; i < segments; i++)
        {
            s.vertex(cos(i * da) * w / 2, sin(i * da) * h / 2);
        }
        s.end(CLOSE);

        this->shape(s, x, y);
    }

    void GraphicsCairo::background(float r, float g, float b, float a)
    {
        Graphics::background(r, g, b, a); // memset(this->data, 0, this->_pixelwidth * this->_pixelheight * 4);
        cairo_set_source_rgba(this->cr, r, g, b, a);
        cairo_paint(this->cr);
    }

    void GraphicsCairo::smooth()
    {
        this->_smooth = true;
        cairo_set_antialias(this->cr, CAIRO_ANTIALIAS_DEFAULT);
    }

    void GraphicsCairo::nosmooth()
    {
        this->_smooth = false;
        cairo_set_antialias(this->cr, CAIRO_ANTIALIAS_NONE);
    }

    void GraphicsCairo::strokecap(int cap)
    {
        if (cap == ROUND)
        {
            cairo_set_line_cap(this->cr, CAIRO_LINE_CAP_ROUND);
        }

        if (cap == SQUARE)
        {
            cairo_set_line_cap(this->cr, CAIRO_LINE_CAP_BUTT);
        }

        if (cap == PROJECT)
        {
            cairo_set_line_cap(this->cr, CAIRO_LINE_CAP_SQUARE);
        }
    }

    void GraphicsCairo::strokejoin(int join)
    {
        if (join == MITER)
        {
            cairo_set_line_join(this->cr, CAIRO_LINE_JOIN_MITER);
        }

        if (join == BEVEL)
        {
            cairo_set_line_join(this->cr, CAIRO_LINE_JOIN_BEVEL);
        }

        if (join == ROUND)
        {
            cairo_set_line_join(this->cr, CAIRO_LINE_JOIN_ROUND);
        }
    }

    // Typography

    void GraphicsCairo::text_impl(std::string text, float x, float y, Unit unit)
    {
        // TODO: Use unit
        cairo_set_source_rgba(this->cr,
                              this->fill_color.r(),
                              this->fill_color.g(),
                              this->fill_color.b(),
                              this->fill_color.a());

        if (this->font != nullptr)
        {
            cairo_set_font_face(this->cr, this->font);
        }
        cairo_set_font_size(this->cr, this->text_size);
        cairo_move_to(this->cr, x, y);
        cairo_show_text(this->cr, text.c_str());
    }

    void GraphicsCairo::textfont_impl(Font *font)
    {
        if (this->font != nullptr)
        {
            cairo_font_face_destroy(this->font);
            this->font = nullptr;
        }
        this->font = cairo_ft_font_face_create_for_ft_face(font->impl->get_ft_face(), 0);
    }
}
