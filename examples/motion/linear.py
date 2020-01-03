import pyro

a = 0

def setup():
    pyro.size(640, 360)
    pyro.stroke(1.0)

def draw():
    global a
    pyro.background(51 / 255.0)
    pyro.line(0, a, pyro.width, a)
    a = a - 0.5
    if a < 0:
        a = pyro.height

pyro.run(setup, draw)
