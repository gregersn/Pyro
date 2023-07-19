import pyro


WIDTH = 10
HEIGHT = 15
DPI = 72
UNIT = pyro.Unit.mm

pg = pyro.create(WIDTH, HEIGHT, UNIT, DPI)


def main():
    print("Hello")


if __name__ == '__main__':
    main()
