#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division

import os
import pyro
import unittest
import numpy as np

# from PIL import Image


class DrawTests(unittest.TestCase):
    def tearDown(self):
        if os.path.isfile('__testfile__.png'):
            os.unlink('__testfile__.png')

    def test_rect(self):
        for h in range(20):
            for w in range(25):
                p = pyro.create(200, 200)
                p.background(0, 0, 0, 1)

                p.nostroke()
                p.fill(1, 1, 1, 1)

                p.rect(50, 50, w, h)

                c = p.get_channels()

                t = np.where(c[:, :, 0] > 0)
                assert len(t[0]) == w * h, len(t[0])
                assert len(t[1]) == w * h, len(t[1])
