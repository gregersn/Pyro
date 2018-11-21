#include <fstream>
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

namespace Pyro {
    json load_json(const std::string &filename) {
        std::ifstream i(filename);
        json j;
        i >> j;

        return j;
    }
    // -I/usr/local/opt/nlohmann_json/include
}
