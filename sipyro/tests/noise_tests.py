#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division

# from pyro.pyro_math import PyroMath
# from pyro import pyro_noise
# from nose.tools import assert_raises

import pyro 

# def test_init():
#     assert pyro_noise.SINCOS_PRECISION == 0.5
#     assert pyro_noise.SINCOS_LENGTH == 720

#     assert pyro_noise.perlin_TWOPI == 720
#     assert pyro_noise.perlin_PI == 360

#     assert len(pyro_noise.COS_LUT) == 720

def test_noise():
    y = 0.0
    x = 0.0
    for y in range(0, 1000):
        for x in range(0, 1000):
            result = pyro.noise(x / 30, y / 30)
            assert result >= 0
            assert result < 1


def test_noise_seed():
    pyro.noise_seed(99)
    # assert len(pyro_noise.perlin) == 0
    # assert pyro_noise.perlinRandom is not None

    resultA = []
    t = 0
    for i in range(5):
        resultA.append(pyro.noise(t))
        t += 0.01

    pyro.noise_seed(99)

    # assert len(pyro_noise.perlin) == 0
    # assert pyro_noise.perlinRandom is not None

    t = 0
    resultB = []
    for i in range(5):
        resultB.append(pyro.noise(t))
        t += 0.01

    for v in resultA:
        assert v < 1
        assert v > 0

    for v in resultB:
        assert v < 1
        assert v > 0

    for i in range(5):
        assert resultA[i] == resultB[i], "results not equal %r: %r %s" % (i, resultA[i], resultB[i])
