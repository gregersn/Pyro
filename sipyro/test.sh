#!/bin/sh

LD_LIBRARY_PATH=./build/src PYTHONPATH=./pybuild/pyro/build/lib.linux-x86_64-3.9/ nosetests
