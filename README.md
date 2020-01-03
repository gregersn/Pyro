# Pyro graphics library

## Graphics library used for generation of images

This library is made for use in my own artistic practise, and comes with no guarnatees of quality.
It is heavly influenced by [Processing](https://processing.org/), but at the time has no functionality for real time graphics and no 3D graphics functionality.

It's primary use is for generating still images with 2D graphics, and functionality is implemented as I need it. 

The state of the whole thing is not really userfriendly at the moment, but hopefully that will improve in the future.

## Dependencies

| Library   | Version | License          |
| --------- | ------- | ---------------- |
| Cairo     | 1.14.x  | LGPL/MPL         |
| SDL       | 2.x     | zlib             |
| libpng    | 1.6.37  | PNG Reference Library License version 2 |       
| Catch2    | 2.9     | BSL-1.0          |

## Building

Using [Meson](https://mesonbuild.com/)

```bash
meson builddir
cd builddir
ninja
```

## Install

```bash
ninja install
```

## Run tests

```bash
ninja test
```

## Build an example

```bash
make ./examples/graphics/piechart
```

## Usage

A very simple example

```c++
#include <pyro/pyro.h>

using namespace Pyro

int main(int argc, char **argv) {
    size(1920, 1080);
    translate(width / 2, height / 2);
    ellipse(0, 0, 50, 50);
    save("test.png");
}
```

Save as `mytest.cpp`, do a `make mytest`, and you should have an executable.

## Tests

Some tests have been implemented using [Catch2](https://github.com/catchorg/Catch2)

## Installing

If you run `make install` at the moment, it is just doing some dumb stuff to get it installed on Mac. 
You can then compile and link a file with something like:
`g++ -MMD -MP -lpyro -std=c++17 somefile.cpp`

## Todo

- Write a better install target
- Make a configure script?
- Write some abstraction to make possible different backends for rendering
