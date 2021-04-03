#include <pyro/pyrographics_gsn.h>

#include <algorithm>
namespace Pyro
{
    GraphicsGSN::GraphicsGSN(unsigned int width, unsigned int height, unsigned int format, unsigned int dpi) : Graphics(width, height, format, 1)
    {
    }

    GraphicsGSN::~GraphicsGSN()
    {
    }

    void GraphicsGSN::shape(Shape s, float x, float y)
    {
        /*cairo_save(this->cr);
        cairo_new_path(this->cr);

        cairo_set_line_width(this->cr, this->stroke_weight);

        cairo_translate(this->cr, x, y);*/

        for (size_t contour_index = 0; contour_index < s.getpoints().size(); contour_index++)
        {
            std::vector<Pyro::Vector> contour_points = s.getpoints()[contour_index];

            for (size_t i = 0; i < contour_points.size(); i++)
            {
                if (i == 0)
                {
                    //cairo_move_to(this->cr, contour_points[i].x, contour_points[i].y);
                }
                else
                {
                    //cairo_line_to(this->cr, contour_points[i].x, contour_points[i].y);
                }
            }
            if (s.close == CLOSE)
            {
                //cairo_close_path(this->cr);
            }
        }
        if (this->fill_enable)
        {
            /*cairo_set_source_rgba(this->cr, this->fill_color.r,
                                  this->fill_color.g,
                                  this->fill_color.b,
                                  this->fill_color.a);
            cairo_fill_preserve(this->cr);*/
        }

        if (this->stroke_enable)
        {
            /*cairo_set_source_rgba(this->cr, this->stroke_color.r,
                                  this->stroke_color.g,
                                  this->stroke_color.b,
                                  this->stroke_color.a);
            cairo_stroke(this->cr);*/
        }

        //cairo_restore(this->cr);
    }

    void GraphicsGSN::line(float x0, float y0, float x1, float y1)
    {
        Pyro::Vector a = this->transformer.apply(Pyro::Vector(x0, y0));
        Pyro::Vector b = this->transformer.apply(Pyro::Vector(x1, y1));
        this->draw_line(a.x, a.y, b.x, b.y);
    }

    void GraphicsGSN::draw_line(float x0, float y0, float x1, float y1)
    {
        if (!this->stroke_enable)
            return;

        float dx = x1 - x0;
        float dy = y1 - y0;

        if (abs(dx) > abs(dy))
        {
            if (x0 > x1)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }

            float a = dy / dx;
            float y = y0;

            for (float x = x0; x <= x1; x++)
            {
                this->set(int(x), int(y), this->stroke_color.to_uint());
                y += a;
            }
        }
        else
        {
            if (y0 > y1)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            float a = dx / dy;
            float x = x0;
            for (float y = y0; y <= y1; y++)
            {
                this->set(int(x), int(y), this->stroke_color.to_uint());
                x += a;
            }
        }
    }
}