#ifndef PYRO_RUNNER_H
#define PYRO_RUNNER_H

namespace Pyro
{
    class Runner
    {
    public:
        void (*keypressed_cb)(){};
        Runner() : running(true){};
        Runner(const Runner &in){};
        // Runner &operator=(const Runner &in) { return *this; };
        virtual ~Runner(){};

        virtual int update()
        {
            return 0;
        };
        virtual int quit()
        {
            return 0;
        };
        virtual int init(unsigned int width, unsigned int height)
        {
            return 0;
        };
        bool running{};

        bool keypressed{}; // True if a key is pressed
        int key{-1};       // Value of key pressed

        void set_keypressed(void (*keypressed)()); // Set callback for keypressed

        bool mousepressed{};
        int mousebutton{-1};
        int mousex{};  // Current horizontal coordinate of the mouse
        int mousey{};  // Current vertical coodrinate of the mouse
        int pmousex{}; // Previous horizontal coordinate of the mouse
        int pmousey{}; // Previous vertical coordinate of the mouse
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

    int get_framecount();

    bool get_keypressed();
    int get_key();
    void set_keypressed(void (*keypressed)());

    bool get_mousepressed();
    int get_mousebutton();
    int get_mousex();
    int get_mousey();
    int get_pmousex();
    int get_pmousey();

    void presetup(bool headless = false);
    void init(unsigned int width, unsigned int height);
    void update();
    void quit();
    void run(void (*setup)(), void (*draw)(), bool headless = false);
    void noloop();
    void loop();
    void stop(); // End runner loop.

    extern bool running;
    extern Runner *pyro;
}

#endif
