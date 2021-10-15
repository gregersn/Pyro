#!/usr/bin/env python3
import re
import sys

UPPER_FUNC_LINE = re.compile(r'^def \w+\(self(, )?.*\)')


def main():
    infile = sys.argv[1]

    with open(infile, 'r') as f:
        data = f.readlines()

        for idx, line in enumerate(data):
            if UPPER_FUNC_LINE.match(line):
                nl = re.sub(r'self(, )?', '', line)
                print(f'{idx + 1} {line.strip()} => {nl.strip()}')
                data[idx] = nl

    with open(infile, 'w') as f:
        f.writelines(data)


if __name__ == "__main__":
    main()
