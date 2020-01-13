#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division

import os
import pyro
import unittest

from PIL import Image


class PixelsTests(unittest.TestCase):
    def setUp(self):
        self.filename = "__testfile__.png"

    def tearDown(self):
        if os.path.isfile(self.filename):
            os.unlink(self.filename)

    def test_red_background(self):
        p = pyro.create(256, 256)
        p.background(1.0, 0.0, 0.0)

        pixel = p.get(128, 128)
        self.assertEqual(pixel, 0xffff0000)

        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (255, 0, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_red_fill(self):
        p = pyro.create(256, 256)
        p.fill(1.0, 0.0, 0.0, 1.0)
        p.nostroke()
        p.rect(0, 0, 256, 256)

        pixel = p.get(128, 128)
        self.assertEqual(pixel, 0xffff0000)


        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (255, 0, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_green_background(self):
        p = pyro.create(256, 256)
        p.background(0.0, 1.0, 0.0, 1.0)

        pixel = p.get(128, 128)
        self.assertEqual(pixel, 0xff00ff00)

        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 255, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_green_fill(self):
        p = pyro.create(256, 256)
        p.fill(0.0, 1.0, 0.0, 1.0)
        p.nostroke()
        p.rect(0, 0, 256, 256)

        pixel = p.get(128, 128)
        self.assertEqual(pixel, 0xff00ff00)

        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 255, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_blue_background(self):
        p = pyro.create(256, 256)
        p.background(0.0, 0.0, 1.0, 1.0)

        pixel = p.get(128, 128)
        self.assertEqual(pixel, 0xff0000ff)

        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 0, 255, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_blue_fill(self):
        p = pyro.create(256, 256)
        p.fill(0.0, 0.0, 1.0, 1.0)
        p.nostroke()
        p.rect(0, 0, 256, 256)

        pixel = p.get(128, 128)
        self.assertEqual(pixel, 0xff0000ff)

        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 0, 255, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_black_background(self):
        p = pyro.create(256, 256)
        p.background(0.0, 0.0, 0.0, 1.0)

        pixel = p.get(128, 128)
        self.assertEqual(pixel, 0xff000000)

        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 0, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_white_background(self):
        p = pyro.create(256, 256)
        p.background(1.0, 1.0, 1.0, 1.0)
        p.save(self.filename)

        i = Image.open(self.filename)
        self.assertEqual(i.mode, 'RGBA')
        pixel = i.getpixel((128, 128))
        assert (255, 255, 255, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_black_fill(self):
        p = pyro.create(256, 256)
        p.fill(0.0, 0.0, 0.0, 1.0)
        p.nostroke()
        p.rect(0, 0, 256, 256)
        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 0, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_white_fill(self):
        p = pyro.create(256, 256)
        p.fill(1.0, 1.0, 1.0, 1.0)
        p.nostroke()
        p.rect(0, 0, 256, 256)
        p.save(self.filename)

        i = Image.open(self.filename)
        self.assertEqual(i.mode, 'RGBA')
        pixel = i.getpixel((128, 128))
        assert (255, 255, 255, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_red_image(self):
        p = pyro.create(256, 256)
        p.background(1.0, 0.0, 0.0, 1.0)
        p.save(self.filename)

        p = pyro.create(256, 256)
        img = p.loadimage(self.filename)
        p.image(img, 0, 0)
        p.save(self.filename)

        i = Image.open(self.filename)
        print(i.mode)
        pixel = i.getpixel((128, 128))
        assert (255, 0, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_green_image(self):
        p = pyro.create(256, 256)
        p.background(0.0, 1.0, 0.0, 1.0)
        p.save(self.filename)

        p = pyro.create(256, 256)
        img = p.loadimage(self.filename)
        p.image(img, 0, 0)
        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 255, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_blue_image(self):
        p = pyro.create(256, 256)
        p.background(0.0, 0.0, 1.0, 1.0)
        p.save(self.filename)

        p = pyro.create(256, 256)
        img = p.loadimage(self.filename)
        p.image(img, 0, 0)
        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 0, 255, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_black_image(self):
        p = pyro.create(256, 256)
        p.background(0.0, 0.0, 0.0, 1.0)
        p.save(self.filename)

        p = pyro.create(256, 256)
        img = p.loadimage(self.filename)
        p.image(img, 0, 0)
        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (0, 0, 0, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()

    def test_white_image(self):
        p = pyro.create(256, 256)
        p.background(1.0, 1.0, 1.0, 1.0)
        p.save(self.filename)

        p = pyro.create(256, 256)
        img = p.loadimage(self.filename)
        p.image(img, 0, 0)
        p.save(self.filename)

        i = Image.open(self.filename)
        pixel = i.getpixel((128, 128))
        assert (255, 255, 255, 255) == pixel,\
            "Pixel is not matching %s" % str(pixel)
        i.close()
