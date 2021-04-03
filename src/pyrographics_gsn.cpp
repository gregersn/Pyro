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
    void GraphicsGSN::shape(Shape s, float x, float y)
    {
        for (uint c = 0; c < s.getpoints().size(); c++)
        {
            auto points = s.getpoints()[c];
            if (this->fill_enable)
            {
                float minx = points[0].x;
                float maxx = points[0].x;
                float miny = points[0].y;
                float maxy = points[0].y;
                for (size_t i = 0; i < points.size(); i++)
                {
                    minx = std::min(minx, points[i].x);
                    miny = std::min(miny, points[i].y);
                    maxx = std::max(maxx, points[i].x);
                    maxy = std::max(maxy, points[i].y);
                }
                for (uint y = miny; y <= maxy; y++)
                {
                    uint32_t col = this->fill_color.to_uint();
                    for (uint x = minx; x <= maxx; x++)
                    {
                        Vector p = Vector(x, y);
                        if (s.is_point_in_path(p))
                        {
                            Vector t = this->transformer.apply(p);
                            this->set(int(t.x), int(t.y), col);
                        }
                    }
                }
            }
            if (this->stroke_enable)
            {
                for (size_t i = 0; i < points.size() - 1; i++)
                {
                    auto p0 = this->transformer.apply(points[i]);
                    auto p1 = this->transformer.apply(points[i + 1]);

                    this->draw_line(p0.x, p0.y, p1.x, p1.y);
                }
                if (s.close == CLOSE)
                {
                    auto p0 = this->transformer.apply(points[points.size() - 1]);
                    auto p1 = this->transformer.apply(points[0]);

                    this->draw_line(p0.x, p0.y, p1.x, p1.y);
                }
            }
        }
    }
}