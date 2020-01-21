cdef extern from "<opencv2/core/mat.hpp>":
    pass

cdef extern from "<tensorflow/core/lib/core/status.h>" namespace "tensorflow":
    cdef cppclass Status:
        Status() except +
        Status OK() except +