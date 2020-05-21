#include <pyro/pyrographics_2d.h>
#include <pyro/pyrocolor.h>
#include <algorithm>

#define ipart(x) (int)(x)
#define round(x) ipart(x + .5)
#define fpart(x) (x -  (int)(x))
#define rfpart(x) (1 - fpart(x))

namespace Pyro {
    uint32_t color_pack(float r, float g, float b, float a) {
        return ((uint)(a * 255) << 24) | ((uint)(r * 255) << 16) | ((uint)(g * 255) << 8) | (uint)(b * 255);
    }

    uint32_t set_alpha(uint32_t color, uint8_t a) {
        return ((a&255) << 24) | (color & 0xffffff);
    }

    uint32_t multiply_alpha(uint32_t color) {
        float a = ((color & 0xff000000) >> 24) / 255.0f;
        float r = std::clamp(((color & 0xff0000) >> 16) / 255.0f * a, 0.0f, 1.0f);
        float g = std::clamp(((color & 0xff00) >> 8) / 255.0f * a, 0.0f, 1.0f);
        float b = std::clamp(((color & 0xff) >> 0) / 255.0f * a , 0.0f, 1.0f);

        return ((uint)(a * 255) << 24) | ((uint)(r * 255) << 16) | ((uint)(g * 255) << 8) | (uint)(b * 255);
    }

    uint32_t a_over_b(uint32_t a, uint32_t b) {
        Color ca = Color::from_uint(a);
        Color cb = Color::from_uint(b);

        Color cc = Color(
            (ca.r * ca.a + cb.r * cb.a * (1 - ca.a)) / (ca.a + cb.a * (1 - ca.a)),
            (ca.g * ca.a + cb.g * cb.a * (1 - ca.a)) / (ca.a + cb.a * (1 - ca.a)),
            (ca.b * ca.a + cb.b * cb.a * (1 - ca.a)) / (ca.a + cb.a * (1 - ca.a)),
            ca.a + cb.a * (1 - ca.a)
        );
        return cc.to_uint();
    }



    Graphics2D::Graphics2D(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi) : Graphics(width, height, channels, dpi), transformer() { 
    }
    Graphics2D::~Graphics2D() { }

    // Coordinates
    void Graphics2D::translate(float x, float y) {
        this->transformer.translate(x, y);
    }

    void Graphics2D::rotate(float a) {
        this->transformer.rotate(a);
    }

    void Graphics2D::scale(float sx, float sy) {
        this->transformer.scale(sx, sy);
    }

    void Graphics2D::pushmatrix() {
        this->transformer.pushmatrix();
    }

    void Graphics2D::popmatrix() {
        this->transformer.popmatrix();
    }

    // Shapes
    void Graphics2D::background(float r, float g, float b, float a) {
        uint32_t color = color_pack(r, g, b, a);
        uint32_t *data = (uint32_t *)this->data;
        for(uint64_t i = 0; i < this->width() * this->height(); i++) {
            data[i] = color;
        }
    }

    void Graphics2D::line(float x0, float y0, float x1, float y1) {
        Pyro::Vector a = this->transformer.apply(Pyro::Vector(x0, y0));
        Pyro::Vector b = this->transformer.apply(Pyro::Vector(x1, y1));
        this->draw_line(a.x, a.y, b.x, b.y);
    }

    void Graphics2D::putpixel(Vector v, float brightness) {
        this->putpixel(v.x, v.y, brightness);
    }

    void Graphics2D::putpixel(Vector v, float brightness, uint32_t color) {
        this->putpixel(v.x, v.y, brightness, color);
    }

    void Graphics2D::putpixel(int x, int y, float brightness) {
        uint32_t color = this->stroke_color.to_uint();
        this->putpixel(x, y, brightness, color);
    }

    void Graphics2D::putpixel(int x, int y, float brightness, uint32_t color) {
        brightness = std::clamp(brightness, 0.0f, 1.0f);
        if (x < 0 || y < 0 || x >= this->_width || y >= this->_height) return;

        color = set_alpha(color, (uint)(brightness * 255));
        uint32_t a = color;

        uint32_t b = ((uint32_t *)this->data)[y * this->_width + x];
        ((uint32_t *)this->data)[y * this->_width + x] = a_over_b(a, b);
    }

    void Graphics2D::draw_line(float x0, float y0, float x1, float y1) {
        int x_0 = round(x0);
        int y_0 = round(y0);
        int x_1 = round(x1);
        int y_1 = round(y1);
        int dx = abs(x_1 - x_0);
        int sx = x_0 < x_1 ? 1 : -1;
        int dy = -abs(y_1 - y_0);
        int sy = y_0 < y_1 ? 1 : -1;
        int err = dx + dy;
        while(true) {
            this->putpixel(x_0, y_0, 255);
            if(x_0 == x_1 && y_0 == y_1) break;
            int e2 = 2 * err;
            if(e2 >= dy) {
                err += dy;
                x_0 += sx;
            }
            if(e2 <= dx) {
                err += dx;
                y_0 += sy;
            }
        }
    }

    void Graphics2D::draw_line_aa(float x0, float y0, float x1, float y1) {
        if(!this->stroke_enable) {
            return;
        }

        x0 -= .5;
        y0 -= .5;
        x1 -= .5;
        y1 -= .5;

        bool steep = fabs(y1 - y0 )> abs(x1 - x0);

        if(steep) {
            std::swap(x0, y0);
            std::swap(x1, y1);
        }

        if(x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        float dx = x1 - x0;
        float dy = y1 - y0;
        float gradient = (dx != 0.0f ? dy / dx : 1.0f);
        
        // First endpoint
        float xend = round(x0);
        float yend = y0 + gradient * (xend - x0);
        float xgap = rfpart(x0 + .5);
        float xpxl1 = xend;
        float ypxl1 = ipart(yend);

        if(steep) {
            this->putpixel(ypxl1, xpxl1, rfpart(yend) * xgap);
            this->putpixel(ypxl1 + 1, xpxl1, fpart(yend) * xgap);
        } else {
            this->putpixel(xpxl1, ypxl1, rfpart(yend) * xgap);
            this->putpixel(xpxl1, ypxl1 + 1, fpart(yend) * xgap);
        }
        float intery = yend + gradient;

        // Second endpoint
        xend = round(x1);
        yend = y1 + gradient * (xend - x1);
        xgap = fpart(x1 + .5);
        float xpxl2 = xend;
        float ypxl2 = ipart(yend);
        if(steep) {
            this->putpixel(ypxl2, xpxl2, rfpart(yend) * xgap);
            this->putpixel(ypxl2 + 1, xpxl2, fpart(yend) * xgap);
        } else 
        {
            this->putpixel(xpxl2, ypxl2, rfpart(yend) * xgap);
            this->putpixel(xpxl2, ypxl2 + 1, fpart(yend) * xgap);
        }

        if(steep) {
            for(uint64_t x = xpxl1 + 1; x < xpxl2; x++) {
                this->putpixel(ipart(intery), x, rfpart(intery));
                this->putpixel(ipart(intery) + 1, x, fpart(intery));
                intery = intery + gradient;                
            }
        } else {
            for(uint64_t x = xpxl1 + 1; x < xpxl2; x++) {
                this->putpixel(x, ipart(intery), rfpart(intery));
                this->putpixel(x, ipart(intery) + 1, fpart(intery));
                intery = intery + gradient;
            }
        }
    }

    void Graphics2D::hline(int y, int x0, int x1, uint32_t col) {
        for(int x = std::min(x0, x1); x <= std::max(x0, x1); x++) {
            this->putpixel(x, y, 1.0f, col);
        }
    }

    void Graphics2D::shape(Shape s, float x, float y) {
        for(uint c = 0; c < s.getpoints().size(); c++) {
            auto points = s.getpoints()[c];
            if(this->fill_enable) {
                float minx = points[0].x;
                float maxx = points[0].x;
                float miny = points[0].y;
                float maxy = points[0].y;
                for(size_t i = 0; i < points.size(); i++) {
                    minx = std::min(minx, points[i].x);
                    miny = std::min(miny, points[i].y);
                    maxx = std::max(maxx, points[i].x);
                    maxy = std::max(maxy, points[i].y);
                }
                for(uint y = miny; y <= maxy; y++) {
                    uint32_t col = this->fill_color.to_uint();
                    for(uint x = minx; x <= maxx; x++) {
                        Vector p = Vector(x, y);
                        if(s.is_point_in_path(p))
                            this->putpixel(this->transformer.apply(p), 1.0f, col);
                    }
                }
            }
            if(this->stroke_enable) {
                for(size_t i = 0; i < points.size() - 1; i++) {
                    auto p0 = this->transformer.apply(points[i]);
                    auto p1 = this->transformer.apply(points[i + 1]);

                    this->draw_line(p0.x, p0.y, p1.x, p1.y);
                }
                if(s.close == CLOSE) {
                    auto p0 = this->transformer.apply(points[points.size() - 1]);
                    auto p1 = this->transformer.apply(points[0]);

                    this->draw_line(p0.x, p0.y, p1.x, p1.y);
                }
            }
        }
    }

    void Graphics2D::image_impl(Image *i, float x, float y) {
        this->fill(255, 0, 0, 255);
        this->rect(x, y, i->width(), i->height());
    }
}
