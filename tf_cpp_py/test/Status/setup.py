from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize


sources = ['PyStatus.pyx']
include = ["/usr/local/include/tensorflow/"]


libs = ["TensorflowShared"]
libdirs = ["/usr/local/lib/cmake/TensorflowCC"]


extensions = [
    Extension("PyStatus",
              sources=sources,
              include_dirs=include,
              libraries=libs,
              library_dirs=libdirs,
             )
]

setup(name='PyStatus',
      ext_modules=cythonize(extensions),
      )