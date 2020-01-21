from setuptools import setup
from distutils.sysconfig import get_python_lib
import glob
import os
import sys

if os.path.exists('readme.rst'):
    print("""The setup.py script should be executed from the build directory.

Please see the file 'readme.rst' for further instructions.""")
    sys.exit(1)


setup(
    name = "cythoncmakeexample",
    package_dir = {'': 'src'},
    data_files = [(get_python_lib(), glob.glob('src/*.so'))],
    author = 'Matt McCormick',
    description = 'Use the CMake build system to make Cython modules.',
    license = 'Apache',
    keywords = 'cmake cython build',
    url = 'http://github.com/thewtex/cython-cmake-example',
    zip_safe = False,
    )
