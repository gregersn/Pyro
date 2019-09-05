#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division

import numpy as np
import numpy.testing as npt

# from pyro import pyro_matrix
# from pyro import pyro_vector


from nose.tools import assert_raises


def test_init():
    t = pyro_matrix.Transform2D()

    cur = t.get()

    assert cur[0, 0] == 1
    assert cur[0, 1] == 0
    assert cur[0, 2] == 0

    assert cur[1, 0] == 0
    assert cur[1, 1] == 1
    assert cur[1, 2] == 0

    assert cur[2, 0] == 0
    assert cur[2, 1] == 0
    assert cur[2, 2] == 1


def test_translate():
    t = pyro_matrix.Transform2D()
    t.translate(3, -4)

    cur = t.get()

    assert cur[0, 2] == 3
    assert cur[1, 2] == -4


def test_rotate():
    t = pyro_matrix.Transform2D()

    t.translate(10, 0)

    cur = t.get()

    assert cur[0, 0] == 1
    assert cur[0, 1] == 0
    assert cur[0, 2] == 10

    assert cur[1, 0] == 0
    assert cur[1, 1] == 1
    assert cur[1, 2] == 0

    t.rotate(np.pi * 2)

    cur = t.get()

    npt.assert_almost_equal(cur[0, 0], 1)
    npt.assert_almost_equal(cur[0, 1], 0)
    assert cur[0, 2] == 10

    npt.assert_almost_equal(cur[1, 0], 0)
    npt.assert_almost_equal(cur[1, 1], 1)
    assert cur[1, 2] == 0

    t.rotate(np.pi)

    cur = t.get()

    assert cur[0, 0] == -1
    npt.assert_almost_equal(cur[0, 1], 0)
    assert cur[0, 2] == 10

    npt.assert_almost_equal(cur[1, 0], 0)
    assert cur[1, 1] == -1
    assert cur[1, 2] == 0


def test_transform_point():
    t = pyro_matrix.Transform2D()

    point = np.array([[0, 0, 1], ]).T

    npoint = t.transform(point)

    npt.assert_array_almost_equal(point, npoint)

    t.rotate(np.pi * 2)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[0, 0, 1], ]).T)

    t.translate(10, -9)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[10, -9, 1], ]).T)

    t.rotate(np.pi * 2)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[10, -9, 1], ]).T)

    t.translate(-10, 9)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[0, 0, 1], ]).T)

    t.rotate(np.pi)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[0, 0, 1], ]).T)

    t.translate(10, -9)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[-10, 9, 1], ]).T)


def test_move_in_rectangle():
    # Test moving in a square and go back to 0,0

    t = pyro_matrix.Transform2D()

    point = np.array([[0, 0, 1], ]).T

    npoint = t.transform(point)

    npt.assert_array_almost_equal(point, npoint)

    t.translate(10, 0)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[10, 0, 1], ]).T)

    t.rotate(np.pi / 2.0)
    t.translate(5, 0)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[10, 5, 1], ]).T)

    t.rotate(np.pi / 2.0)
    t.translate(10, 0)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[0, 5, 1], ]).T)

    t.rotate(np.pi / 2.0)
    t.translate(5, 0)
    npoint = t.transform(point)
    npt.assert_array_almost_equal(npoint, np.array([[0, 0, 1], ]).T)


def test_push_pop():
    t = pyro_matrix.Transform2D()

    v1 = pyro_vector.Vector2D(0, 0)
    v2 = v1.copy()

    v1.transform(t.get())

    npt.assert_array_almost_equal(v1.data, v2.data)

    t.push()

    t.translate(10, -5)
    t.rotate(1.63323)
    t.translate(3, 4)

    v1 = v2.copy()
    v1.transform(t.get())

    assert_raises(AssertionError,
                  npt.assert_array_almost_equal,
                  v1.data, v2.data)

    t.pop()
    v1 = v2.copy()
    v1.transform(t.get())

    npt.assert_array_almost_equal(v1.data, v2.data)

    # Do a stack underflow(?) test
    assert_raises(BufferError,
                  t.pop)
