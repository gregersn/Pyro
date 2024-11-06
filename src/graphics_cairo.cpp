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

    void GraphicsCairo::shape(Shape s, float x, float y)
    {
        // TODO: Use unit
        cairo_save(this->cr);
        cairo_new_path(this->cr);

        cairo_set_line_width(this->cr, this->style.strokeweight());

        cairo_translate(this->cr, x * pixel_multiplier, y * pixel_multiplier);

        for (size_t contour_index{0}; contour_index < s.getpoints().size(); contour_index++)
        {
            std::vector<Pyro::Vector> contour_points{s.getpoints()[contour_index]};

            for (size_t i{0}; i < contour_points.size(); i++)
            {
                if (i == 0)
                {
                    cairo_move_to(this->cr, contour_points[i].x * pixel_multiplier, contour_points[i].y * pixel_multiplier);
                }
                else
                {
                    cairo_line_to(this->cr, contour_points[i].x * pixel_multiplier, contour_points[i].y * pixel_multiplier);
                }
            }
            if (s.close == CLOSE)
            {
                cairo_close_path(this->cr);
            }
        }
        if (this->style.fill_enabled())
        {
            cairo_set_source_rgba(this->cr, this->style.fill().r(),
                                  this->style.fill().g(),
                                  this->style.fill().b(),
                                  this->style.fill().a());
            cairo_fill_preserve(this->cr);
        }

        if (this->style.stroke_enabled())
        {
            cairo_set_source_rgba(this->cr, this->style.stroke().r(),
                                  this->style.stroke().g(),
                                  this->style.stroke().b(),
                                  this->style.stroke().a());
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
    void GraphicsCairo::translate(float x, float y)
    {
        cairo_translate(this->cr, x * pixel_multiplier, y * pixel_multiplier);
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

    void GraphicsCairo::line(float x0, float y0, float x1, float y1)
    {
        if (this->style.stroke_enabled())
        {
            cairo_new_path(this->cr);
            cairo_move_to(this->cr, x0 * pixel_multiplier, y0 * pixel_multiplier);
            cairo_line_to(this->cr, x1 * pixel_multiplier, y1 * pixel_multiplier);
            cairo_set_line_width(this->cr, this->style.strokeweight());
            cairo_set_source_rgba(this->cr,
                                  this->style.stroke().r(),
                                  this->style.stroke().g(),
                                  this->style.stroke().b(),
                                  this->style.stroke().a());
            cairo_stroke(this->cr);
        }
    }

    void GraphicsCairo::background(float r, float g, float b, float a)
    {
        Graphics::background(r, g, b, a);
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
        Graphics::strokecap(cap);
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
        Graphics::strokejoin(join);
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

    void GraphicsCairo::popstyle()
    {
        Graphics::popstyle();
        this->strokecap(this->style.strokecap());
        this->strokejoin(this->style.strokejoin());
    }

    // Typography

    void GraphicsCairo::text_impl(std::string text, float x, float y)
    {
        cairo_set_source_rgba(this->cr,
                              this->style.fill().r(),
                              this->style.fill().g(),
                              this->style.fill().b(),
                              this->style.fill().a());

        if (this->font != nullptr)
        {
            cairo_set_font_face(this->cr, this->font);
        }
        cairo_set_font_size(this->cr, this->style.textsize());
        cairo_move_to(this->cr, x * pixel_multiplier, y * pixel_multiplier);
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
