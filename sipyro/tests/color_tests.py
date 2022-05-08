#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
import pyro


class TestColors(unittest.TestCase):

    def test_rgba_1(self):
        c = pyro.Color(0.0)

        self.assertAlmostEqual(c.r(), 0.0)
        self.assertAlmostEqual(c.g(), 0.0)
        self.assertAlmostEqual(c.b(), 0.0)
        self.assertAlmostEqual(c.a(), 1.0)

    def test_rgba_2(self):
        c = pyro.Color(0.4, 0.5)

        self.assertAlmostEqual(c.r(), 0.4)
        self.assertAlmostEqual(c.g(), 0.4)
        self.assertAlmostEqual(c.b(), 0.4)
        self.assertAlmostEqual(c.a(), 0.5)

    def test_rgba_3(self):
        c = pyro.Color(0.1, 0.2, 0.3)

        self.assertAlmostEqual(c.r(), 0.1)
        self.assertAlmostEqual(c.g(), 0.2)
        self.assertAlmostEqual(c.b(), 0.3)
        self.assertAlmostEqual(c.a(), 1.0)

    def test_rgba_4(self):
        c = pyro.Color(0.9, 0.8, 0.6, 0.5)

        self.assertAlmostEqual(c.r(), 0.9)
        self.assertAlmostEqual(c.g(), 0.8)
        self.assertAlmostEqual(c.b(), 0.6)
        self.assertAlmostEqual(c.a(), 0.5)

    def test_fto32(self):
        u = pyro.Color.fto32(1.0, 1.0, 1.0, 1.0)
        self.assertEqual(u, 0xffffffff)

        u = pyro.Color.fto32(0.0, 0.0, 0.0, 0.0)
        self.assertEqual(u, 0x00000000)

        u = pyro.Color.fto32(0.0, 0.0, 0.0, 1.0)
        self.assertEqual(u, 0xff000000)

        u = pyro.Color.fto32(1.0, 0.0, 0.0, 1.0)
        self.assertEqual(u, 0xffff0000)

        u = pyro.Color.fto32(1.0, 0.0, 1.0, 1.0)
        self.assertEqual(u, 0xffff00ff)

        u = pyro.Color.fto32(1.0, 0.0, 1.0, 0.5)
        self.assertEqual(u, 0x7fff00ff)

    def test_init_with_hex(self):
        """Can be initialized with hex-strings."""
        a = pyro.Color("#00ffff")
        self.assertAlmostEqual(a.r(), 0.0)
        self.assertAlmostEqual(a.g(), 1.0)
        self.assertAlmostEqual(a.b(), 1.0)
        self.assertAlmostEqual(a.a(), 1.0)

    def test_init_opaque_with_one_parameter(self):
        """Will initialize as opaque with one parameter."""
        a = pyro.Color(0.0)
        self.assertAlmostEqual(a.r(), 0.0)
        self.assertAlmostEqual(a.g(), 0.0)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

    def test_init_transparent_grayscale(self):
        """Will intialize as transparent greyscale with two parameters."""
        a = pyro.Color(0.4, 0.5)
        self.assertAlmostEqual(a.r(), 0.4)
        self.assertAlmostEqual(a.g(), 0.4)
        self.assertAlmostEqual(a.b(), 0.4)
        self.assertAlmostEqual(a.a(), 0.5)

    def test_init_as_solid(self):
        """Will initialize as solid colors with three parameters"""
        a = pyro.Color(0.1, 0.2, 0.3)
        self.assertAlmostEqual(a.r(), 0.1)
        self.assertAlmostEqual(a.g(), 0.2)
        self.assertAlmostEqual(a.b(), 0.3)
        self.assertAlmostEqual(a.a(), 1.0)

    def test_init_with_alpha(self):
        """Will initialize with alpha with four parameters."""
        a = pyro.Color(0.1, 0.2, 0.3, 0.5)
        self.assertAlmostEqual(a.r(), 0.1)
        self.assertAlmostEqual(a.g(), 0.2)
        self.assertAlmostEqual(a.b(), 0.3)
        self.assertAlmostEqual(a.a(), 0.5)

    def test_init_from_uints(self):
        """Will initialize with four uint."""
        a = pyro.Color(255, 0, 0, 255)
        self.assertAlmostEqual(a.r(), 1.0)
        self.assertAlmostEqual(a.g(), 0.0)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

    def test_convert_to_32bit_uint(self):
        """Will convert to uint32."""
        a = pyro.Color(1.0, 0.0, 1.0, 1.0)
        c = a.to_uint()
        self.assertAlmostEqual(c, 0xffff00ff)

    def test_uint_conversion(self):
        """Can be converted to and from uint32."""
        a = pyro.Color.from_uint(0xff00ff00)
        self.assertAlmostEqual(a.r(), 0.0)
        self.assertAlmostEqual(a.g(), 1.0)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

    def test_lerping(self):
        """Colors can be lerped."""
        a = pyro.Color(1.0, 0.0, 0.0, 1.0)
        b = pyro.Color(0.0, 1.0, 0.0, 0.0)

        c = a.lerp(b, 0.5)

        self.assertAlmostEqual(c.r(), 0.5)
        self.assertAlmostEqual(c.g(), 0.5)
        self.assertAlmostEqual(c.b(), 0.0)
        self.assertAlmostEqual(c.a(), 0.5)

    def test_set_with_hsb(self):
        """Colors can be set with HSB."""
        a = pyro.Color(1.0)
        a.hsba(0.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 1.0)
        self.assertAlmostEqual(a.g(), 0.0)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(0.125 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 1.0)
        self.assertAlmostEqual(a.g(), 0.25)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(0.25 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 1.0)
        self.assertAlmostEqual(a.g(), 0.5)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(0.5 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 1.0)
        self.assertAlmostEqual(a.g(), 1.0)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(0.75 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 0.5)
        self.assertAlmostEqual(a.g(), 1.0)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(1.0 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 0.0)
        self.assertAlmostEqual(a.g(), 1.0)
        self.assertAlmostEqual(a.b(), 0.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(1.5 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 0.0)
        self.assertAlmostEqual(a.g(), 1.0)
        self.assertAlmostEqual(a.b(), 1.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(2.0 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 0.0)
        self.assertAlmostEqual(a.g(), 0.0)
        self.assertAlmostEqual(a.b(), 1.0)
        self.assertAlmostEqual(a.a(), 1.0)

        a.hsba(2.5 / 3.0, 1.0, 1.0, 1.0)
        self.assertAlmostEqual(a.r(), 1.0)
        self.assertAlmostEqual(a.g(), 0.0)
        self.assertAlmostEqual(a.b(), 1.0)
        self.assertAlmostEqual(a.a(), 1.0)
