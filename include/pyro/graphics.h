#ifndef PYROGRAPHICS_H
#define PYROGRAPHICS_H

#include "constants.h"
#include "color.h"
#include "image.h"
#include "shape.h"
#include "font.h"
#include "transformer.h"

namespace Pyro
{
    enum GraphicsMode
    {
        CAIRO
    };

    class Graphics : public Image
    {
    protected:
        bool stroke_enable{true};
        bool fill_enable{true};

        bool _smooth{true};

        Color stroke_color{Color(0.0f, 1.0f)};
        Color fill_color{Color(1.0f, 1.0f)};
        int _rect_mode{Pyro::CORNER};
        int _image_mode{Pyro::CORNER};
        int _ellipse_mode{Pyro::CENTER};
        float stroke_weight{1.0f};
        float text_size{16.0f};

        Shape _shape{Shape()};
        Transformer2D transformer;

    public:
        Graphics(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi, Unit unit = Unit::PX);
        virtual ~Graphics() override;

        virtual void init() override;

        static Graphics *create(unsigned int width, unsigned int height, GraphicsMode mode = GraphicsMode::CAIRO, unsigned int dpi = 72, Unit unit = Unit::PX);

        void imagemode(int mode)
        {
            this->_image_mode = mode;
        };
        void image(Image *img, float x, float y);
        virtual void image_impl(Image * /*img*/, float /*x*/, float /*y*/) {};
        Image *loadimage(std::string const &filename) { return Image::load(filename); };

        // Color functions
        void nostroke();
        void nofill();

        virtual void blendmode(int /*mode*/) {};
        virtual void colormode(int mode)
        {
            this->stroke_color.colormode(mode);
            this->fill_color.colormode(mode);
        };

        void fill(Color c) { this->fill(c.r(), c.g(), c.b(), c.a()); };

        void fill(float c, float a = 1.0) { this->fill(c, c, c, a); };
        void fill(float r, float g, float b, float a = 1.0);

        void fill(int c, int a = 255);
        void fill(int r, int g, int b, int a = 255);

        void stroke(Color c) { this->stroke(c.r(), c.g(), c.b(), c.a()); };

        void stroke(float c) { this->stroke(c, c, c, 1.0); };
        void stroke(float c, float a) { this->stroke(c, c, c, a); };
        void stroke(float r, float g, float b) { this->stroke(r, g, b, 1.0); };
        void stroke(float r, float g, float b, float a);

        void stroke(int c, int a = 255);
        void stroke(int r, int g, int b, int a = 255);

        void strokeweight(float w, Unit unit = Unit::CURRENT);
        virtual void strokecap(int cap);
        virtual void strokejoin(int join);

        virtual void smooth();
        virtual void nosmooth();

        // Transformation
        virtual void translate(float x, float, Unit unit = Unit::CURRENT);
        virtual void rotate(float a);
        virtual void scale(float sx, float sy);
        virtual void pushmatrix();
        virtual void popmatrix();
        virtual float screen_x(float x, float y, float z = 0.0f);
        virtual float screen_y(float x, float y, float z = 0.0f);

        // Drawing functions
        void background(Color c)
        {
            this->background(c.r(), c.g(), c.b(), c.a());
        };
        void background(int c, int a = 255)
        {
            this->background(c / 255.0f, c / 255.0f, c / 255.0f, a / 255.0);
        };
        void background(int r, int g, int b, int a = 255)
        {
            this->background(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        };

        void background(float c, float a = 1.0)
        {
            this->background(c, c, c, a);
        };
        virtual void background(float r, float g, float b, float a = 1.0);

        virtual void shape(Shape /*s*/, float /*x*/, float /*y*/, Unit /*unit*/ = Unit::CURRENT) {};

        void beginshape(int kind = DEFAULT)
        {
            this->_shape.begin(kind);
        };
        void begincontour()
        {
            this->_shape.begincontour();
        };
        void endcontour()
        {
            this->_shape.endcontour();
        };
        void vertex(Vector v)
        {
            this->_shape.vertex(v.x, v.y);
        };
        void vertex(float x, float y)
        {
            this->_shape.vertex(x, y);
        };
        void curvevertex(float x, float y)
        {
            this->_shape.curvevertex(x, y);
        };
        void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4)
        {
            this->_shape.beziervertex(x2, y2, x3, y3, x4, y4);
        }
        void endshape(int close);
        void endshape()
        {
            endshape(0);
        };

        // Primitive shapes
        void point(float x, float y, Unit unit = Unit::CURRENT);
        virtual void line(float /*x0*/, float /*y0*/, float /*x1*/, float /*y1*/, Unit /*unit*/ = Unit::CURRENT) {};
        void curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, Unit unit = Unit::CURRENT);
        void bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, Unit unit = Unit::CURRENT);

        void triangle(Vector a, Vector b, Vector c);
        void triangle(float x0, float y0, float x1, float y1, float x2, float y2, Unit unit = Unit::CURRENT);

        void rectmode(int mode)
        {
            this->_rect_mode = mode;
        };
        void rect(float a, float b, float c, float d, Unit unit = Unit::CURRENT);

        void quad(Vector a, Vector b, Vector c, Vector d);
        void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, Unit unit = Unit::CURRENT);

        void arc(float a, float b, float c, float d, float start, float end, int mode = OPEN, Unit unit = Unit::CURRENT);

        void ellipsemode(int mode)
        {
            this->_ellipse_mode = mode;
        };
        void ellipse(float x, float y, float w, float h, unsigned int segments, Unit unit = Unit::CURRENT);
        void ellipse(float x, float y, float r, unsigned int segments, Unit unit = Unit::CURRENT)
        {
            this->ellipse(x, y, r, r, segments, unit);
        };

        // Typography

        void textsize(float size, Unit unit = Unit::CURRENT);
        void text(std::string const &text, float x, float y, Unit unit = Unit::CURRENT);
        void textfont(Font *font);

        virtual void textfont_impl(Font * /*font*/) {};
        virtual void text_impl(std::string /*text*/, float /*x*/, float /*y*/, Unit /*unit*/ = Unit::CURRENT) {};
    };
};

#endif
