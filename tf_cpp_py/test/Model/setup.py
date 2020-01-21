from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize


sources = ['PyModel.pyx']
include = []

libs = ["opencv_calib3d", "opencv_core", "opencv_dnn", "opencv_features2d", "opencv_flann", "opencv_gapi", "opencv_highgui", "opencv_imgcodecs", "opencv_imgproc", "opencv_ml", "opencv_objdetect", "opencv_photo", "opencv_stitching", "opencv_video", "opencv_videoio", "TensorflowShared"]
libs = []
libdirs = ["/usr/local/include/opencv4", "/usr/local/lib/cmake/TensorflowCC", "usr/local/include/tensorflow"]


extensions = [
    Extension("PyModel",
              sources=sources,
              include_dirs=include,
              libraries=libs,
              library_dirs=libdirs,
             )
]

setup(name='PyModel',
      ext_modules=cythonize(extensions),
      )