#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division

from PIL import Image
import numpy as np
import unittest
import os

import pyro


def split_32bit(a: int):
    return ((a & 0xff0000) >> 16,
            (a & 0xff00) >> 8,
            (a & 0xff),
            (a & 0xff000000) >> 24)


def compare_pixels(a, b):
    print(type(a))
    print("We want: ", a)
    print("To equal: ", b)
    if isinstance(a, int):
        return compare_pixels(split_32bit(a), b)

    if not len(a) == len(b):
        return False

    for i in range(len(a)):
        if a[i] != b[i]:
            return False

    return True


def test_createimage():
    img = Pyro.createimage(320, 240, Pyro.RGB)
    assert img.width() == 320
    assert img.height() == 240
    # assert img.mode() == Pyro.RGB


class ResizeTests(unittest.TestCase):
    def setUp(self):
        self.img = pyro.Image.load('../tests/Lenna.png')

    def test_resize(self):
        img = self.img
        img = img.resize(256, 128)

        assert img.width() == 256
        assert img.height() == 128

    def test_resize_aspect_w(self):
        img = self.img
        img = img.resize(200, 0)
        assert img.width() == 200
        assert img.height() == 200

    def test_resize_aspect_h(self):
        img = self.img
        img = img.resize(0, 231)

        assert img.width() == 231
        assert img.height() == 231


class PixelTests(unittest.TestCase):
    def setUp(self):
        self.img = pyro.Image.load('../tests/Lenna.png')

    def test_get(self):
        pixel = self.img.get(0, 0)
        assert pixel is not None
        assert compare_pixels(pixel, (226, 137, 125, 255))

        pixel = self.img.get(511, 0)
        assert pixel is not None
        assert compare_pixels(pixel, (200, 99, 90, 255))

        pixel = self.img.get(511, 511)
        assert pixel is not None
        assert compare_pixels(pixel, (185, 74, 81, 255))

        pixel = self.img.get(0, 511)
        assert pixel is not None
        assert compare_pixels(pixel, (82, 22, 57, 255))

    def test_get_area(self):
        """
        pixel = self.img.get(0, 0, 1, 1)
        assert pixel is not None
        assert compare_pixels(pixel, (226, 137, 125, 255))

        pixel = self.img.get(511, 0, 1, 1)
        assert pixel is not None
        assert compare_pixels(pixel, (200, 99, 90, 255))

        pixel = self.img.get(511, 511, 1, 1)
        assert pixel is not None
        assert compare_pixels(pixel, (185, 74, 81, 255))

        pixel = self.img.get(0, 511, 1, 1)
        assert pixel is not None
        assert compare_pixels(pixel, (82, 22, 57, 255))
        """
        pixels = self.img.get(0, 0, 3, 2)
        assert pixels.height() == 2
        assert pixels.width() == 3

        assert compare_pixels(pixels[0], (226, 137, 125, 255))
        assert compare_pixels(pixels[1], (226, 137, 125, 255))
        assert compare_pixels(pixels[pixels.width()], (226, 137, 125, 255))
        assert compare_pixels(pixels[pixels.width() + 1], (226, 137, 125, 255))


class ImageTests(unittest.TestCase):
    def tearDown(self):
        if os.path.isfile('__testfile__.png'):
            os.unlink('__testfile__.png')

    def test_init(self):
        img = Pyro.createimage(10, 11, Pyro.RGB)

        assert img.load_pixels() is not None
        assert img.load_pixels().shape[0] == 11
        assert img.load_pixels().shape[1] == 10
        assert img.load_pixels().shape[2] == 3
        assert img.load_pixels().dtype == np.uint8

        pixels = img.load_pixels()
        pixels[0, 0, 0] = 128

        assert pixels[0, 0, 0] == 128

        pixels = img.load_pixels()
        assert pixels[0, 0, 0] == 128

    def test_load(self):
        img = pyro.Image.load('../tests/Lenna.png')
        assert img.width() == 512
        assert img.height() == 512

        pixels = img.load_pixels()
        assert pixels.shape[0] == 512
        assert pixels.shape[1] == 512
        assert pixels.shape[2] == 8
        assert pixels.dtype == np.uint8, ("Pixels dtype %s" % pixels.dtype)

    def test_create_save_load(self):
        img = Pyro.createimage(4, 4, 4)
        img.save("__test_image__.png")
        i = Image.open("__test_image__.png")

        self.assertEqual("RGBA", i.mode)

    # def test_getchannels(self):
    #     img = pyro.Image.load('examples/Lenna.png')

    #     channels = img.get_channels()

    #     assert channels.shape == (512, 512, 4)
    #     print(channels.shape)
    #     print(channels[:, :, 3])
    #     assert False
