#include <pyro.h>

using namespace Pyro;

int main(int argc, char **argv) {
    size(200, 100);

    {
        Image *img = createimage(66, 66, RGB);
        unsigned int *pixels = img->load_pixels();
        for(int i = 0; i < 66 * 66; i++) {
            pixels[i] = color(0u, 90u, 102u);
        }
        img->update_pixels();
        image(img, 17, 17);

        delete(img);
    }

    translate(100, 0);
    
    {
        Image *img = createimage(66, 66, RGBA);
        unsigned int *pixels = img->load_pixels();
        for(int i = 0; i < 66 * 66; i++) {
            pixels[i] = color(0u, 90u, 102u, i % img->width() * 2);
        }
        img->update_pixels();

        image(img, 17, 17);
        image(img, 34, 34);

        delete(img);
    }
    save("output/image.png");
    return 0;
}