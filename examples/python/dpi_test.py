import pyro


def main():
    screen_size = pyro.create(100, 100, dpi=72, unit=pyro.Unit.mm)
    screen_size.ellipse(50,
                        50, 20, 20, 8)

    screen_size.save('screen_size.png')

    double_size = pyro.create(50, 50, dpi=144, unit=pyro.Unit.mm)
    double_size.ellipse(25,
                        25, 20, 20, 8)

    double_size.save('double_size.png')


if __name__ == '__main__':
    main()
