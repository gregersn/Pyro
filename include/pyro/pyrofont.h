#ifndef PYROFONT_H
#define PYROFONT_H
#include <string>

namespace Pyro {
    class Font {
        private:
            std::string filename = "";
        public:
            static Font * load(std::string filename);
    };
}
#endif
