import unittest

import pyro


class TestGraphicsInit(unittest.TestCase):

    def test_create(self):
        graphics = pyro.Graphics.create(400, 300)

        self.assertEqual(graphics.width(), 400)
        self.assertEqual(graphics.height(), 300)


class TestGraphicsBackground(unittest.TestCase):

    def setUp(self):
        self.pg = pyro.Graphics.create(4, 4)

    def test_red_background(self):
        self.pg.background(1.0, 0.0, 0.0)
        p = self.pg.get(0, 0)

        self.assertEqual(p, 0xffff0000)


class TestGraphicsStroke(unittest.TestCase):

    def setUp(self):
        self.pg = pyro.Graphics.create(10, 1)

    def test_no_stroke(self):
        self.pg.background(255)
        self.pg.nostroke()
        self.pg.nofill()
        self.pg.line(0, 0, 10, 0)
        p = self.pg.get(0, 0)

        self.assertEqual(p, 0xffffffff)
