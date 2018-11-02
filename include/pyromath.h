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

        template <typename T>
        T lerp(T start, T stop, float amt) {
            return (stop * amt) + (start * (1.0 - amt));
        };

        template<typename T>
        T constrain(T value, T low, T high) {
            T t = value <= high ? value : high;
            return t >= low ? t : low;
        };

        
        float radians(float degree);

        static std::default_random_engine rng = std::default_random_engine(0);
        static std::uniform_real_distribution<double> rng_dist = std::uniform_real_distribution(0.0, 1.0);

        unsigned int random(unsigned int range);
        unsigned int random(unsigned int low, unsigned int high);

        int random(int range);
        int random(int low, int high);

        double random(); // Returns a random number between 0 and 1
        double random(double range);
        double random(double low, double high);

        void randomseed(unsigned int seed);


}

#endif
