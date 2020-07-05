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
import unittest

# def test_init():
#     assert pyro_noise.SINCOS_PRECISION == 0.5
#     assert pyro_noise.SINCOS_LENGTH == 720

#     assert pyro_noise.perlin_TWOPI == 720
#     assert pyro_noise.perlin_PI == 360

#     assert len(pyro_noise.COS_LUT) == 720

class NoiseTests(unittest.TestCase):
    def test_noise(self):
        y = 0.0
        x = 0.0
        for y in range(0, 1000):
            for x in range(0, 1000):
                result = pyro.noise(x / 30, y / 30)
                self.assertGreaterEqual(result, 0)
                self.assertLess(result, 1)


    def test_noise_seed(self):
        pyro.noiseseed(99)
        # assert len(pyro_noise.perlin) == 0
        # assert pyro_noise.perlinRandom is not None

        resultA = []
        t = 0
        for i in range(5):
            resultA.append(pyro.noise(t, 0.0))
            t += 0.01

        pyro.noiseseed(99)

        # assert len(pyro_noise.perlin) == 0
        # assert pyro_noise.perlinRandom is not None

        t = 0
        resultB = []
        for i in range(5):
            resultB.append(pyro.noise(t, 0.0))
            t += 0.01

        for v in resultA:
            self.assertLess(v, 1)
            self.assertGreater(v, 0)

        for v in resultB:
            self.assertLess(v, 1)
            self.assertGreater(v, 0)


        for i in range(5):
            self.assertEqual(resultA[i], resultB[i], "results not equal %r: %r %s" % (i, resultA[i], resultB[i]))
