import os
import sys
import unittest


class SetupTests(unittest.TestCase):
    def test_env(self):
        python_path = os.environ.get("PYTHONPATH")
        ld_library_path = os.environ.get("LD_LIBRARY_PATH")
        cwd = os.getcwd()
        paths = sys.path

        try:
            import pyro
        except Exception as e:
            self.assertTrue(False)


