#ifndef PYRO_RUNNER_H
#define PYRO_RUNNER_H

#include <functional>
#include <stdexcept>

namespace Pyro
{
    class Runner
    {
    public:
        std::function<void()> keypressed_cb{nullptr};
        std::function<void()> mousepressed_cb{nullptr};
        Runner() : running(true){};
        Runner(const Runner &/*in*/){};
        Runner &operator=(const Runner &/*in*/) { return *this; };
        virtual ~Runner(){};

        virtual int update()
        {
            throw std::logic_error("Not implemented");
        };
        virtual int quit()
        {
            throw std::logic_error("Not implemented");
        };
        virtual int init(unsigned int /*width*/, unsigned int /*height*/)
        {
            throw std::logic_error("Not implemented");
        };
        bool running;

        bool keypressed; // True if a key is pressed
        int key;         // Value of key pressed

        void set_keypressed(std::function<void()> keypressed); // Set callback for keypressed
        void set_mousepressed(std::function<void()> mousepressed); // Set callback for mousepressed

        bool mousepressed;
        int mousebutton;
        int mousex;  // Current horizontal coordinate of the mouse
        int mousey;  // Current vertical coodrinate of the mouse
        int pmousex; // Previous horizontal coordinate of the mouse
        int pmousey; // Previous vertical coordinate of the mouse
    };

    // State variables
    extern unsigned int framecount;
    extern bool keypressed;
    extern int key;

    extern bool mousepressed;
    extern int mousebutton;
    extern int mousex;
    extern int mousey;
    extern int pmousex;
    extern int pmousey;

    bool get_keypressed();
    int get_key();
    void set_keypressed(std::function<void()> keypressed);

    bool get_mousepressed();
    void set_mousepressed(std::function<void()> mousepressed);
    int get_mousebutton();
    int get_mousex();
    int get_mousey();
    int get_pmousex();
    int get_pmousey();

    void presetup(bool headless = false);
    void init(unsigned int width, unsigned int height);
    void update();
    void quit();
    void run(const std::function<void()> &setup, const std::function<void()> &draw, bool headless = false);
    void noloop();
    void loop();
    void stop(); // End runner loop.

    extern bool running;
    extern Runner *pyro;
}

#endif
