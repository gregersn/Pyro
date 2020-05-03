#include <pyro/pyrographics_2d.h>

#define ipart(x) (int)(x)
#define round(x) ipart(x + .5)
#define fpart(x) (x -  (int)(x))
#define rfpart(x) (1 - fpart(x))

namespace Pyro {
    uint32_t Graphics2D::color_pack(float r, float g, float b, float a) {
        return ((uint)(a * 255) << 24) | ((uint)(r * 255) << 16) | ((uint)(g * 255) << 8) | (uint)(b * 255);
    }

    Graphics2D::Graphics2D(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi) : Graphics(width, height, channels, dpi) { }
    Graphics2D::~Graphics2D() { }

    void Graphics2D::background(float r, float g, float b, float a) {
        uint32_t color = color_pack(r, g, b, a);
        uint32_t *data = (uint32_t *)this->data;
        for(uint64_t i = 0; i < this->width() * this->height(); i++) {
            data[i] = color;
        }
    }

    void Graphics2D::line(float x0, float y0, float x1, float y1) {
        if(!this->stroke_enable) {
            return;
        }

        uint32_t color = this->stroke_color.to_uint();
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
            this->set(ypxl1, xpxl1, color);
            this->set(ypxl1 + 1, xpxl1, color);
        } else {
            this->set(xpxl1, ypxl1, color);
            this->set(xpxl1, ypxl1 + 1, color);
        }
        float intery = yend + gradient;

        // Second endpoint
        xend = round(x1);
        yend = y1 + gradient * (xend - x1);
        xgap = fpart(x1 + .5);
        float xpxl2 = xend;
        float ypxl2 = ipart(yend);
        if(steep) {
            this->set(ypxl2, xpxl2, color);
            this->set(ypxl2 + 1, xpxl2, color);
        } else 
        {
            this->set(xpxl2, ypxl2, color);
            this->set(xpxl2, ypxl2 + 1, color);
        }

        if(steep) {
            for(uint64_t x = xpxl1 + 1; x < xpxl2; x++) {
                this->set(ipart(intery), x, color);
                this->set(ipart(intery) + 1, x, color);
                intery = intery + gradient;                
            }
        } else {
            for(uint64_t x = xpxl1 + 1; x < xpxl2; x++) {
                this->set(x, ipart(intery), color);
                this->set(x, ipart(intery) + 1, color);
                intery = intery + gradient;
            }
        }
    }

    void Graphics2D::shape(Shape s, float x, float y) {
        for(size_t i = 0; i < s.getpoints().size() - 1; i++) {
            auto p0 = s.getpoints()[i];
            auto p1 =  s.getpoints()[i + 1];

            this->line(p0.x, p0.y, p1.x, p1.y);
        }
        if(s.close == CLOSE) {
            auto p0 = s.getpoints()[s.getpoints().size() - 1];
            auto p1 =  s.getpoints()[0];
            this->line(p0.x, p0.y, p1.x, p1.y);
        }
    }
}
