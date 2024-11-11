#ifndef PYROGRAPHICS_H
#define PYROGRAPHICS_H

#include "constants.h"
#include "color.h"
#include "image.h"
#include "shape.h"
#include "font.h"
#include "transformer.h"
#include "style.h"

#include <filesystem>

namespace Pyro
{
    enum GraphicsMode
    {
        CAIRO,
        PDF,
        SVG,
    };

    class Graphics : public Image
    {
    protected:
        GraphicsMode mode;
        std::filesystem::path filename{""};

        int _smooth{3};
        unsigned int _curvedetail{32};

        Shape _shape{Shape()};
        Transformer2D transformer;
        StyleStack style;

    public:
        Graphics(unsigned int width, unsigned int height, std::filesystem::path filename = "");
        virtual ~Graphics() override;

        virtual void init() override;

        void imagemode(int mode)
        {
            this->style.imagemode(mode);
        };
        void image(Image *img, float x, float y);
        virtual void image_impl(Image * /*img*/, float /*x*/, float /*y*/) {};
        Image *loadimage(std::filesystem::path const &filename) { return Image::load(filename); };

        // Color functions
        void nostroke();
        void nofill();

        virtual void blendmode(int /*mode*/) {};
        void colormode(int mode)
        {
            this->style.colormode(mode);
        };

        Color fill() { return this->style.fill(); };
        Color fill(Color c) { return this->fill(c.r(), c.g(), c.b(), c.a()); };

        Color fill(float c, float a = 1.0) { return this->fill(c, c, c, a); };
        Color fill(float r, float g, float b, float a = 1.0);

        Color fill(int c, int a = 255);
        Color fill(int r, int g, int b, int a = 255);

        Color stroke() { return this->style.stroke(); };
        Color stroke(Color c) { return this->stroke(c.r(), c.g(), c.b(), c.a()); };

        Color stroke(float c) { return this->stroke(c, c, c, 1.0); };
        Color stroke(float c, float a) { return this->stroke(c, c, c, a); };
        Color stroke(float r, float g, float b) { return this->stroke(r, g, b, 1.0); };
        Color stroke(float r, float g, float b, float a);

        Color stroke(int c, int a = 255);
        Color stroke(int r, int g, int b, int a = 255);

        void curvedetail(unsigned int segments);

        float strokeweight();
        float strokeweight(float w);
        virtual void strokecap(int cap);
        virtual void strokejoin(int join);

        virtual void smooth();
        virtual void smooth(int level);
        virtual void nosmooth();

        // Transformation
        virtual void translate(float x, float y);
        virtual void translate(Vector v);
        virtual void rotate(float a);
        virtual void scale(float sx, float sy);
        virtual void scale(Vector v);
        virtual void pushmatrix();
        virtual void popmatrix();
        void pushstyle();
        virtual void popstyle();
        virtual float screen_x(float x, float y, float z = 0.0f);
        virtual float screen_y(float x, float y, float z = 0.0f);

        virtual void begindraw() { load_pixels(); };
        virtual void enddraw() { update_pixels(); };

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

        virtual void shape(Shape /*s*/, float /*x*/, float /*y*/) {};

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
        void vertex(Vector p)
        {
            this->_shape.vertex(p);
        };
        void vertex(float x, float y)
        {
            this->_shape.vertex(x, y);
        };
        void curvevertex(Vector p)
        {
            this->_shape.curvevertex(p);
        };
        void curvevertex(float x, float y)
        {
            this->_shape.curvevertex(x, y);
        };
        void beziervertex(Vector p2, Vector p3, Vector p4)
        {
            this->_shape.beziervertex(p2, p3, p4);
        }
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
        void point(float x, float y);
        void point(Vector p);
        virtual void line(Vector /*p0*/, Vector /*p1*/){};
        virtual void line(float /*x0*/, float /*y0*/, float /*x1*/, float /*y1*/) {};
        void curve(Vector p0, Vector p1, Vector p2, Vector p3);
        void curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
        void bezier(Vector p0, Vector p1, Vector p2, Vector p3);
        void bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

        void triangle(Vector a, Vector b, Vector c);
        void triangle(float x0, float y0, float x1, float y1, float x2, float y2);

        void rectmode(int mode)
        {
            this->style.rectmode(mode);
        };
        void rect(float a, float b, float c, float d);
        void rect(Vector p0, Vector p1);

        void quad(Vector a, Vector b, Vector c, Vector d);
        void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

        void arc(Vector p0, Vector p1, float start, float end, int mode = OPEN);
        void arc(float a, float b, float c, float d, float start, float end, int mode = OPEN);

        void ellipsemode(int mode)
        {
            this->style.ellipsemode(mode);
        };
        void ellipse(float x, float y, float w, float h);
        void ellipse(float x, float y, float r)
        {
            this->ellipse(x, y, r, r);
        };
        void ellipse(Vector p0, float w, float h);
        void ellipse(Vector p0, float r)
        {
            this->ellipse(p0, r, r);
        };

        // Typography

        void textsize(float size);
        void text(std::string const &text, float x, float y);
        void textfont(Font *font);

        virtual void textfont_impl(Font * /*font*/) {};
        virtual void text_impl(std::string /*text*/, float /*x*/, float /*y*/) {};
    };
    /**
     * Create a graphics object
     *
     * @param width The width of the canvas
     * @param height The height of the canvas
     */
    Graphics *creategraphics(unsigned int width,
                             unsigned int height,
                             GraphicsMode mode = GraphicsMode::CAIRO,
                             std::filesystem::path filename = "");

    Graphics *creategraphics(unsigned int width, unsigned int height, int dpi, Unit unit, GraphicsMode mode = GraphicsMode::CAIRO, std::filesystem::path filename = "");
};

#endif
