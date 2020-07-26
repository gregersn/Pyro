#ifndef PYRO_RUNNER_H
#define PYRO_RUNNER_H

namespace Pyro {
    class Runner {
    public:
        Runner() : running(true) {};
        Runner(const Runner &in) { };
        Runner(unsigned int width, unsigned int height) : running(true) {};
        Runner & operator=(const Runner &in) { return *this; };
        virtual ~Runner() {};

        virtual int update() { throw; return 0; };
        virtual int quit() { throw; return 0; };
        virtual int init() { throw; return 0; };
        bool running;
    };
    extern unsigned int framecount;

    void init();
    void update();
    void quit();
    void run(void (*setup)(), void (*draw)());
    void noloop();
    void loop();

    extern bool running;
    extern Runner *pyro;
}

#endif