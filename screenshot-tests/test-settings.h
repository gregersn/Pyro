#ifndef SCREENSHOT_SETTINGS_H
#define SCREENSHOT_SETTINGS_H
#include <catch2/catch_all.hpp>
#include "pyro/pyro.h"
#include <filesystem>

static inline std::filesystem::path const actual_folder = "./screenshots/expected/";
static inline std::filesystem::path const current_folder = "./screenshots/current/";
constexpr static inline Pyro::GraphicsMode testmode = Pyro::GraphicsMode::CAIRO;

class ImageMatch : public Catch::Matchers::MatcherBase<std::filesystem::path>
{
    std::filesystem::path actual_file;
    float tolerance = 0.0f;

public:
    ImageMatch(std::filesystem::path const &actual) : actual_file(actual) {};
    ImageMatch(std::filesystem::path const &actual, float tolerance) : actual_file(actual), tolerance(tolerance) {};
    bool match(std::filesystem::path const &filename) const override;
    virtual std::string describe() const override;
};

inline ImageMatch LooksLike(std::filesystem::path const &actual)
{
    return ImageMatch(actual);
}

inline ImageMatch LooksLike(std::filesystem::path actual, float tolerance)
{
    return ImageMatch(actual, tolerance);
}

#endif
