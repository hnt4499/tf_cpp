
set(CMAKE_CXX_STANDARD 11)
# If the pyx file is a C++ file, we should specify that here.
set_source_files_properties(
  ${CYTHON_CMAKE_EXAMPLE_SOURCE_DIR}/src/model.pyx
  PROPERTIES CYTHON_IS_CXX TRUE)

# ==================== PATHS TO SPECIFY! ==================== #

# Eigen lib headers
include_directories("/usr/local/include/eigen3")

# TensorFlow headers
include_directories("/usr/local/include/tf/")
include_directories("/usr/local/include/tf/bazel-genfiles/")
include_directories("/usr/local/include/tf/tensorflow/")
include_directories("/usr/local/include/tf/third-party/")

# Link TensorFlow libs
link_libraries("/usr/local/lib/libtensorflow_cc.so")
link_libraries("/usr/local/lib/libtensorflow_framework.so")

# Multi-file cython modules do not appear to be working at the moment.
cython_add_module(model model.pyx Model.cpp)
