from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "pyro",
        sorted(glob("src/*.cpp")),
        include_dirs=["../include", "/usr/local/include"],
        extra_compile_args=['-lpyro']
    ),
]

setup(name="pyro", cmdclass={
      "build_ext": build_ext}, ext_modules=ext_modules, )
