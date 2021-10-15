#!/bin/sh
cd sipyro
sip-build --pep484-pyi
cd ..
python3 ./tools/fix_python_types.py ./sipyro/build/pyro/pyro.pyi
