#ifndef SCREENSHOT_SETTINGS_H
#define SCREENSHOT_SETTINGS_H
#include <string>
#include <catch2/catch.hpp>
#include <pyro/pyro.h>

static inline std::string const actual_folder = "../screenshots/expected/";
static inline std::string const current_folder = "../screenshots/current/";
constexpr static inline Pyro::GraphicsMode testmode = Pyro::GraphicsMode::P2D;

class ImageMatch : public Catch::MatcherBase<std::string>
{
    std::string actual_file;
    float tolerance = 0.0f;

public:
    ImageMatch(std::string actual) : actual_file(actual){};
    ImageMatch(std::string actual, float tolerance) : actual_file(actual), tolerance(tolerance){};
    bool match(std::string const &filename) const override;
    virtual std::string describe() const override;
};

inline ImageMatch LooksLike(std::string actual)
{
    return ImageMatch(actual);
}

inline ImageMatch LooksLike(std::string actual, float tolerance)
{
    return ImageMatch(actual, tolerance);
}

#endif