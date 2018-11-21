#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Pyro {
    json load_json(const std::string &filename);
}
