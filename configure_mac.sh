#!/bin/bash

export LDFLAGS="-L/usr/local/opt/jpeg-turbo/lib"
export CPPFLAGS="-I/usr/local/opt/jpeg-turbo/include"
export PKG_CONFIG_PATH="/usr/local/opt/jpeg-turbo/lib/pkgconfig:/usr/local/opt/libffi/lib/pkgconfig"
export CC=gcc-13
export CXX=g++-13
meson setup build
