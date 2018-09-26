#ifndef PYROGRAPHICS_H
#define PYROGRAPHICS_H

#include <pyroconstants.h>
#include <pyrocolor.h>
#include <pyroimage.h>
#include <pyroshape.h>

#include <cairo/cairo.h>

namespace Pyro {
    class Graphics : public Image {
        cairo_surface_t *surface;
        cairo_t *cr;

        bool stroke_enable;
        bool fill_enable;

        bool _smooth;

        t_color stroke_color;
        t_color fill_color;
        float stroke_weight;

        Shape _shape;

        public:
            Graphics(unsigned int width, unsigned int height, unsigned int bpp);
            ~Graphics();

            static Graphics *create(unsigned int width, unsigned int height);
            //void save(const std::string &file);

            unsigned int random(unsigned int range);
            unsigned int random(unsigned int low, unsigned int high);

            int random(int range);
            int random(int low, int high);

            float random(float range);
            float random(float low, float high);

            void image(Image *img, float x, float y);

            // Color functions
            void nostroke();
            void nofill();
            
            void fill(float c) { this->fill(c, c, c, 1.0); };
            void fill(float c, float a) {this->fill(c, c, c, a); };
            void fill(float r, float g, float b) {this->fill(r, g, b, 1.0); };
            void fill(float r, float g, float b, float a);

            void fill(int c);
            void fill(int c, int a);
            void fill(int r, int g, int b);
            void fill(int r, int g, int b, int a);

            void stroke(float c) { this->stroke(c, c, c, 1.0); };
            void stroke(float c, float a) {this->stroke(c, c, c, a); };
            void stroke(float r, float g, float b) {this->stroke(r, g, b, 1.0); };
            void stroke(float r, float g, float b, float a);

            void stroke(int c);
            void stroke(int c, int a);
            void stroke(int r, int g, int b);
            void stroke(int r, int g, int b, int a);

            void strokeweight(float w); 
            void strokecap(int cap);

            void smooth();
            void nosmooth();

            // Transformation
            void translate(float x, float);
            void rotate(float a);
            void pushmatrix();
            void popmatrix();

            // Drawing functions
            void background(int c) { this->background(c / 255.0f, c / 255.0f, c / 255.0f, 1.0f); };
            void background(int c, int a) { this->background(c / 255.0f, c / 255.0f, c / 255.0f, a / 255.0); };
            void background(int r, int g, int b) { this->background(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f); };
            void background(int r, int g, int b, int a) { this->background(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); };

            void background(float c) { this->background(c, c, c, 1.0); };
            void background(float c, float a) { this->background(c, c, c, a); };
            void background(float r, float g, float b) { this->background(r, g, b, 1.0); };
            void background(float r, float g, float b, float a);

            void shape(Shape s, float x, float y);

            void beginshape() {this->_shape.begin(); };
            void vertex(float x, float y) { this->_shape.vertex(x, y); };
            void endshape(int close);

            // Primitive shapes
            void point(float x, float y);
            void line(float x0, float y0, float x1, float y1);
            void triangle(float x0, float y0, float x1, float y1, float x2, float y2);
            void rect(float a, float b, float c, float d);
            void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void arc(float a, float b, float c, float d, float start, float end) { this->arc(a, b, c, d, start, end, OPEN); };
            void arc(float a, float b, float c, float d, float start, float end, int mode);

            void ellipse(float x, float y, float w, float h);
            void ellipse(float x, float y, float r) { this->ellipse(x, y, r, r); };

            // Math functions
            template <typename T>
            T map(T value, T start1, T stop1, T start2, T stop2) {
                return ((value - start1) * (stop2 - start2) / (stop1 - start1)) + start2;
            };
            
            float radians(float degree);

    };
};

#endif