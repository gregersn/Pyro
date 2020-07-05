import sys
import pyro
import time

a = 0

def setup():
    global a
    pyro.size(640, 360);
    pyro.stroke(255);
    a = pyro.height / 2;

def draw():
    global a
    pyro.background(51);
    pyro.line(0, a, pyro.width, a);
    a = a - 0.5;
    if a < 0:
        a = pyro.height

pyro.run(setup, draw)

