#!/bin/bash

export LDFLAGS="-L/usr/local/opt/jpeg-turbo/lib"
export CPPFLAGS="-I/usr/local/opt/jpeg-turbo/include"
export PKG_CONFIG_PATH="/usr/local/opt/jpeg-turbo/lib/pkgconfig:/usr/local/opt/libffi/lib/pkgconfig"
export CC=x86_64-apple-darwin22-gcc-13
export CXX=x86_64-apple-darwin22-g++-13
meson setup build
