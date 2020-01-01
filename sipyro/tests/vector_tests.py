#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import numpy.testing as npt
import unittest

import pyro


class TestVector(unittest.TestCase):
    def test_init(self):
        v = pyro.Vector(1, 2, 3)

        assert v.x == 1
        assert v.y == 2
        assert v.z == 3

    def test_setters_and_getters(self):
        v = pyro.Vector(0, 0, 0)
        v.x = 1
        v.y = 2
        v.z = 3

        assert v.x == 1
        assert v.y == 2
        assert v.z == 3

    def test_angle_between(self):
        v1 = pyro.Vector(0, 10)
        v2 = pyro.Vector(10, 0)
        assert (v1.angle(v2) - (3.1415926 / 2)) < 0.0001

        v1 = pyro.Vector(0, 10)
        v2 = pyro.Vector(0, 10)
        assert (v1.angle(v2)) < 0.0001

        v1 = pyro.Vector(0, -10)
        v2 = pyro.Vector(0, 10)
        assert (v1.angle(v2) - 3.1415926) < 0.0001

        v1 = pyro.Vector(0, 10)
        v2 = pyro.Vector(10, 10)
        assert (v1.angle(v2) - (3.1415926 / 4)) < 0.0001


class Arithmetics(unittest.TestCase):
    def setUp(self):
        self.v1 = pyro.Vector(1, 2, 3)
        self.v2 = pyro.Vector(3, 2, 1)

    def test_add(self):

        v3 = self.v1 + self.v2

        # print(v3.data)

        assert v3.x == 4
        assert v3.y == 4
        assert v3.z == 4

        v3 = self.v1 + self.v2

        assert v3.x == 4
        assert v3.y == 4
        assert v3.z == 4

        v3 = self.v2 + self.v1

        assert v3.x == 4
        assert v3.y == 4
        assert v3.z == 4

    def test_sub(self):
        v3 = self.v1 - self.v2

        assert v3.x == -2
        assert v3.y == 0
        assert v3.z == 2

    def test_mult(self):
        v3 = self.v2 * 2

        assert v3.x == 6
        assert v3.y == 4
        assert v3.z == 2

    def test_div(self):
        v3 = self.v2 / 2

        assert v3.x == 1.5
        assert v3.y == 1
        assert v3.z == 0.5

    def test_mag(self):
        v = pyro.Vector(0, 0, 0).mag()
        assert v == 0

        v = pyro.Vector(0, 1, 0).mag()
        assert v == 1

        v = pyro.Vector(4, 3, 0).mag()
        assert v == 5

    def test_setmag(self):
        v = pyro.Vector(4, 3, 2)
        v = v.setmag(4)
        self.assertAlmostEqual(v.mag(), 4)

    def test_normalize(self):
        self.v2 = self.v2.normalize()
        self.assertAlmostEqual(self.v2.mag(), 1.0, 5)

        self.v1 = self.v1.normalize()
        self.assertAlmostEqual(self.v1.mag(), 1.0, 5)

    def test_limit(self):
        v = pyro.Vector(1, 0, 0)

        v = v.limit(2)
        assert v.mag() == 1.0

        v = v.limit(1)
        assert v.mag() == 1.0

        v = v.limit(0.5)
        assert v.mag() == 0.5

    def test_random2d(self):
        v = pyro.Vector.random2d()
        assert abs(v.mag() - 1.0) < 0.00001

    def test_random3d(self):
        v = pyro.Vector.random3d()
        assert abs(v.mag() - 1.0) < 0.00001
