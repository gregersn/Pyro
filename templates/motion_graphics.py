import pyro

WIDTH = 640
HEIGHT = 480


def setup():
    pyro.size(WIDTH, HEIGHT)


pos_x: float = 0


def draw():
    global pos_x
    pyro.background(192)
    pyro.ellipse(pos_x, pyro.height / 2, 10, 10, 16)
    pos_x += 1
    pos_x %= pyro.width


if __name__ == '__main__':
    pyro.run(setup, draw)
