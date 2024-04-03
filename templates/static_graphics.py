"""Static pyro skech template."""
from pathlib import Path

import pyro


WIDTH = 10
HEIGHT = 15
DPI = 72
UNIT = pyro.Unit.mm

def main():
    """Main graphics creatiion."""
    pg = pyro.create(WIDTH, HEIGHT, UNIT, DPI)
    pg.save(f"{Path(__file__).stem}_{WIDTH}{UNIT.name}_x_{HEIGHT}{UNIT.name}_@_{DPI}_dpi.png", DPI)


if __name__ == '__main__':
    main()
