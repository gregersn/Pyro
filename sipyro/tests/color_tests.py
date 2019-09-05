#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
from pyro import Pyro

pyrocolor = Pyro.Color

class TestColors(unittest.TestCase):
    def test_rgba_1(self):
        c = pyrocolor(0.0)

        self.assertAlmostEqual(c.r, 0.0)
        self.assertAlmostEqual(c.g, 0.0)
        self.assertAlmostEqual(c.b, 0.0)
        self.assertAlmostEqual(c.a, 1.0)


    def test_rgba_2(self):
        c = pyrocolor(0.4, 0.5)

        
        self.assertAlmostEqual(c.r, 0.4)
        self.assertAlmostEqual(c.g, 0.4)
        self.assertAlmostEqual(c.b, 0.4)
        self.assertAlmostEqual(c.a, 0.5)


    def test_rgba_3(self):
        c = pyrocolor(0.1, 0.2, 0.3)

        self.assertAlmostEqual(c.r, 0.1)
        self.assertAlmostEqual(c.g, 0.2)
        self.assertAlmostEqual(c.b, 0.3)
        self.assertAlmostEqual(c.a, 1.0)


    def test_rgba_4(self):
        c = pyrocolor(0.9, 0.8, 0.6, 0.5)

        self.assertAlmostEqual(c.r, 0.9)
        self.assertAlmostEqual(c.g, 0.8)
        self.assertAlmostEqual(c.b, 0.6)
        self.assertAlmostEqual(c.a, 0.5)


    def test_fto32(self):
        u = pyrocolor.fto32(1.0, 1.0, 1.0, 1.0)
        assert u == 0xffffffff

        u = pyrocolor.fto32(0.0, 0.0, 0.0, 0.0)
        assert u == 0x00000000

        u = pyrocolor.fto32(0.0, 0.0, 0.0, 1.0)
        assert u == 0xff000000

        u = pyrocolor.fto32(1.0, 0.0, 0.0, 1.0)
        assert u == 0xffff0000

        u = pyrocolor.fto32(1.0, 0.0, 1.0, 1.0)
        assert u == 0xffff00ff

        u = pyrocolor.fto32(1.0, 0.0, 1.0, 0.5)
        assert u == 0x7fff00ff

    def test_hsb_2_rgb(self):
        c = pyrocolor.hsba(0.0, 0.0, 0.0, 1.0)

        self.assertAlmostEqual(c.r, 0.0)
        self.assertAlmostEqual(c.g, 0.0)
        self.assertAlmostEqual(c.b, 0.0)
        self.assertAlmostEqual(c.a, 1.0)

        c = pyrocolor.hsba(0.0, 1.0, 1.0, 1.0)

        self.assertAlmostEqual(c.r, 1.0)
        self.assertAlmostEqual(c.g, 0.0)
        self.assertAlmostEqual(c.b, 0.0)
        self.assertAlmostEqual(c.a, 1.0)
        

