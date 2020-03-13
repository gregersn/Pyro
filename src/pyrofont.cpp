#include <pyro/pyrofont.h>

namespace Pyro {
    Font *Font::load(std::string filename) {
        Font *f = new Font();
        f->filename = filename;
        return f;
    }
}