#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division

from pyro import Pyro

import unittest


class TestMath(unittest.TestCase):
    def test_map(self):
        assert Pyro.map(0, 0, 100, 0, 1) == 0
        assert Pyro.map(100, 0, 100, 0, 1) == 1

    def test_lerp(self):
        assert Pyro.lerp(0, 5, 0) == 0
        assert Pyro.lerp(0, 5, 1) == 5
        assert Pyro.lerp(0, 5, 0.5) == 2.5

    def test_degrees(self):
        self.assertAlmostEqual(Pyro.degrees(0), 0)
        self.assertAlmostEqual(Pyro.degrees(Pyro.HALF_PI), 90, places=5)
        self.assertAlmostEqual(Pyro.degrees(Pyro.PI), 180, places=5)

    def test_radians(self):
        self.assertAlmostEqual(Pyro.degrees(Pyro.radians(0)), 0,
                               places=5)
        self.assertAlmostEqual(Pyro.degrees(Pyro.radians(90)), 90,
                               places=5)
        self.assertAlmostEqual(Pyro.degrees(Pyro.radians(180)), 180,
                               places=5)
        self.assertAlmostEqual(Pyro.degrees(Pyro.radians(270)), 270,
                               places=5)
