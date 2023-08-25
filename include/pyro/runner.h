#ifndef PYRO_RUNNER_H
#define PYRO_RUNNER_H

namespace Pyro
{
    class Runner
    {
    public:
        void (*keypressed_cb)();
        Runner() : running(true){};
        Runner(const Runner &in){};
        Runner &operator=(const Runner &in) { return *this; };
        virtual ~Runner(){};

        virtual int update()
        {
            throw;
            return 0;
        };
        virtual int quit()
        {
            throw;
            return 0;
        };
        virtual int init(unsigned int width, unsigned int height)
        {
            throw;
            return 0;
        };
        bool running;
        bool keypressed;
        int key;
        void set_keypressed(void (*keypressed)());
    };
    extern unsigned int framecount;
    extern bool keypressed;
    extern int key;

    bool get_keypressed();
    int get_key();
    void set_keypressed(void (*keypressed)());

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
