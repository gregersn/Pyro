#ifndef PYROSTYLE_H
#define PYROSTYLE_H

#include "color.h"

namespace Pyro
{
    struct t_style
    {
        // Colors
        Color fill{Color(1.0f, 1.0f)};
        Color stroke{Color(0.0f, 1.0f)};
        Color tint{Color(1.0f, 0.0f, 0.0f, 1.0f)};

        bool stroke_enable{true};
        bool fill_enable{true};

        // Stroke
        float stroke_weight{1.0f};
        int stroke_cap;
        int stroke_join;

        // Modes
        int image_mode{Pyro::CORNER};
        int rect_mode{Pyro::CORNER};
        int ellipse_mode{Pyro::CENTER};
        //  int shape_mode; // Not implemented
        int color_mode{Pyro::RGB};

        // Test
        // int text_align;
        // int text_font;
        int text_mode;
        float text_size{16.0f};
        // int text_leading; // Not implemented

        // Lighting
        /*int emissive;
        int specular;
        int shininess;
        int ambient;*/
    };

    class StyleStack
    {
    private:
        std::vector<t_style> stack;
        t_style current;

    public:
        StyleStack();
        void push();
        void pop();

        int imagemode();

        void imagemode(int mode);
        void colormode(int mode);

        int rectmode();
        void rectmode(int mode);
        void ellipsemode(int mode);

        float textsize();

        void textsize(float size);
        float strokeweight();
        void strokeweight(float w);
        void nostroke();
        bool stroke_enabled();
        void nofill();
        bool fill_enabled();
        Color fill();
        void fill(float r, float g, float b, float a);
        Color stroke();
        void stroke(float r, float g, float b, float a);
        void strokecap(int cap);
        int strokecap();
        void strokejoin(int join);
        int strokejoin();
    };
}

#endif