#include <pybind11/pybind11.h>
#include <pyro/pyro.h>

PYBIND11_MODULE(pyro, m) {
    m.doc() = "pyro Python bindings";
//    m.def("create", &Pyro::create, "Create the graphics object");
}
