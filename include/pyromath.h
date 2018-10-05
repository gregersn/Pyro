#ifndef PYROMATH_H
#define PYROMATH_H

#include <random>

namespace Pyro {
        template <typename T>
        inline T min(T a, T b) { return a ? a < b : b; };
        template<typename T>
        inline T max(T a, T b) { return a ? a > b : b; };

        template <typename T>
        T map(T value, T start1, T stop1, T start2, T stop2) {
            return ((value - start1) * (stop2 - start2) / (stop1 - start1)) + start2;
        };
        
        float radians(float degree);

        static std::default_random_engine rng = std::default_random_engine(0);
        static std::uniform_real_distribution<float> rng_dist = std::uniform_real_distribution(0.0f, 1.0f);

        unsigned int random(unsigned int range);
        unsigned int random(unsigned int low, unsigned int high);

        int random(int range);
        int random(int low, int high);

        float random(); // Returns a random number between 0 and 1
        float random(float range);
        float random(float low, float high);

        void randomseed(unsigned int seed);


}

#endif
