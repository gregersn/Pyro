#ifndef SCREENSHOT_SETTINGS_H
#define SCREENSHOT_SETTINGS_H
#include <string>
#include <catch2/catch.hpp>

std::string actual_folder = "../screenshot-tests/actual/";
std::string current_folder = "../screenshot-tests/current/";

class ImageMatch : public Catch::MatcherBase<std::string> {
    std::string actual_file;

public:
    ImageMatch(std::string actual) : actual_file(actual) {};
    bool match(std::string const& filename) const override;
    virtual std::string describe() const override;
};

inline ImageMatch LooksLike(std::string actual) {
    return ImageMatch(actual);
}
#endif 