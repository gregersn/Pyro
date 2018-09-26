#ifndef PYROCOLOR_H
#define PYROCOLOR_H


namespace Pyro {
    struct t_color {
        float r;
        float g;
        float b;
        float a;
    };
    
    class Color {
        public:
            float r;
            float g;
            float b;
            float a;
            Color(float r, float g, float b) {
                this->r = r;
                this->g = g;
                this->b = b;
                this->a = 1.0f;
            }

            Color(float r, float g, float b, float a) {
                this->r = r;
                this->g = g;
                this->b = b;
                this->a = a;
            }

            Color(unsigned int r, unsigned int g, unsigned int b) {
                this->r = r / 255.0f;
                this->g = g / 255.0f;
                this->b = b / 255.0f;
                this->a = 1.0f;
            }
    };


}

#endif