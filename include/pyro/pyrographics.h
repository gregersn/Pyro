#ifndef PYROGRAPHICS_H
#define PYROGRAPHICS_H

#include "pyroconstants.h"
#include "pyrocolor.h"
#include "pyroimage.h"
#include "pyroshape.h"
#include "pyrofont.h"

namespace Pyro {
    enum class BlendMode {
        ADD,
        BLEND,
        DARKEST,
        DIFFERENCE,
        EXCLUSION,
        LIGHTEST,
        MULTIPLY,
        REPLACE,
        SCREEN,
        SUBTRACT
    };

    enum class GraphicsMode {
        CAIRO
    };

    class Graphics : public Image {
        protected:
            bool stroke_enable = true;
            bool fill_enable = true;

            bool _smooth = true;

            Color stroke_color = Color(0.0f, 1.0f);
            Color fill_color = Color(1.0f, 1.0f);
            int _rect_mode = Pyro::CORNER;
            int _image_mode = Pyro::CORNER;
            int _ellipse_mode = Pyro::CENTER;
            float stroke_weight =  1.0f;
            float text_size = 16.0f;

            Shape _shape = Shape();

        public:
            Graphics(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
            virtual ~Graphics();

            static Graphics *create(unsigned int width, unsigned int height);
            static Graphics *create(unsigned int width, unsigned int height, GraphicsMode mode);
            static Graphics *create(unsigned int width, unsigned int height, GraphicsMode mode, unsigned int dpi);

            void imagemode(int mode) { this->_image_mode = mode; };
            void image(Image *img, float x, float y);
            virtual void image_impl(Image *img, float x, float y) {};
            inline Image *loadimage(std::string filename) { return Image::load(filename); };

            // Color functions
            void nostroke();
            void nofill();

            virtual void blendmode(BlendMode mode) {};
            virtual void colormode(int mode) {
                this->stroke_color.colormode(mode);
                this->fill_color.colormode(mode);
            };

            inline void fill(Color c) { this->fill(c.r, c.g, c.b, c.a); };
            
            inline void fill(float c) { this->fill(c, c, c, 1.0); };
            inline void fill(float c, float a) {this->fill(c, c, c, a); };
            inline void fill(float r, float g, float b) {this->fill(r, g, b, 1.0); };
            void fill(float r, float g, float b, float a);

            void fill(int c);
            void fill(int c, int a);
            void fill(int r, int g, int b);
            void fill(int r, int g, int b, int a);

            inline void stroke(Color c) { this->stroke(c.r, c.g, c.b, c.a); };

            inline void stroke(float c) { this->stroke(c, c, c, 1.0); };
            inline void stroke(float c, float a) {this->stroke(c, c, c, a); };
            inline void stroke(float r, float g, float b) {this->stroke(r, g, b, 1.0); };
            void stroke(float r, float g, float b, float a);

            void stroke(int c);
            void stroke(int c, int a);
            void stroke(int r, int g, int b);
            void stroke(int r, int g, int b, int a);

            void strokeweight(float w); 
            virtual void strokecap(int cap);
            virtual void strokejoin(int join);

            virtual void smooth();
            virtual void nosmooth();

            // Transformation
            virtual void translate(float x, float) {};
            virtual void rotate(float a) {};
            virtual void scale(float sx, float sy) {};
            virtual void pushmatrix() {};
            virtual void popmatrix() {};

            // Drawing functions
            inline void background(Color c) { this->background(c.r, c.g, c.b, c.a); };
            inline void background(int c) { this->background(c / 255.0f, c / 255.0f, c / 255.0f, 1.0f); };
            inline void background(int c, int a) { this->background(c / 255.0f, c / 255.0f, c / 255.0f, a / 255.0); };
            inline void background(int r, int g, int b) { this->background(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f); };
            inline void background(int r, int g, int b, int a) { this->background(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); };

            inline void background(float c) { this->background(c, c, c, 1.0); };
            inline void background(float c, float a) { this->background(c, c, c, a); };
            inline void background(float r, float g, float b) { this->background(r, g, b, 1.0f); };
            virtual void background(float r, float g, float b, float a);

            virtual void shape(Shape s, float x, float y) {};

            void beginshape() {this->_shape.begin(); };
            void begincontour() {this->_shape.begincontour(); };
            void endcontour() { this->_shape.endcontour(); };
            void vertex(Vector v) { this->_shape.vertex(v.x, v.y); };
            void vertex(float x, float y) { this->_shape.vertex(x, y); };
            void curvevertex(float x, float y) { this->_shape.curvevertex(x, y); };
            void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) { this->_shape.beziervertex(x2, y2, x3, y3, x4, y4); }
            void endshape(int close);
            inline void endshape() { endshape(0); };

            // Primitive shapes
            void point(float x, float y);
            virtual void line(float x0, float y0, float x1, float y1) {};
            void curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
            void bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void triangle(Vector a, Vector b, Vector c);
            void triangle(float x0, float y0, float x1, float y1, float x2, float y2);
    
            void rectmode(int mode) { this->_rect_mode = mode; };
            void rect(float a, float b, float c, float d);

            void quad(Vector a, Vector b, Vector c, Vector d);
            void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void arc(float a, float b, float c, float d, float start, float end) { this->arc(a, b, c, d, start, end, OPEN); };
            void arc(float a, float b, float c, float d, float start, float end, int mode);

            void ellipsemode(int mode) { this->_ellipse_mode = mode; };
            void ellipse(float x, float y, float w, float h, unsigned int segments);
            void ellipse(float x, float y, float r, unsigned int segments) { this->ellipse(x, y, r, r, segments); };

            // Typography

            void textsize(float size);
            void text(std::string text, float x, float y);
            void textfont(Font *font);

            virtual void textfont_impl(Font *font) {};
            virtual void text_impl(std::string text, float x, float y) {};

    };
};

#endif