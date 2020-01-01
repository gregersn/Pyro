#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division

import pyro

import unittest


class TestMath(unittest.TestCase):
    def test_map(self):
        assert pyro.map(0, 0, 100, 0, 1) == 0
        assert pyro.map(100, 0, 100, 0, 1) == 1

    def test_lerp(self):
        assert pyro.lerp(0, 5, 0) == 0
        assert pyro.lerp(0, 5, 1) == 5
        assert pyro.lerp(0, 5, 0.5) == 2.5

    def test_degrees(self):
        self.assertAlmostEqual(pyro.degrees(0), 0)
        self.assertAlmostEqual(pyro.degrees(pyro.HALF_PI), 90, places=5)
        self.assertAlmostEqual(pyro.degrees(pyro.PI), 180, places=5)

    def test_radians(self):
        self.assertAlmostEqual(pyro.degrees(pyro.radians(0)), 0,
                               places=5)
        self.assertAlmostEqual(pyro.degrees(pyro.radians(90)), 90,
                               places=5)
        self.assertAlmostEqual(pyro.degrees(pyro.radians(180)), 180,
                               places=5)
        self.assertAlmostEqual(pyro.degrees(pyro.radians(270)), 270,
                               places=5)
