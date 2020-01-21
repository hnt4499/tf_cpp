# distutils: language = c++

from c_model cimport Model as CModel

cdef class Model:
    cdef CModel *model_ptr  # Hold a C++ pointer which we're wrapping
    cpdef say(self)
