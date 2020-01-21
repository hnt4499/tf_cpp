# distutils: language = c++

from Status cimport Status

cdef class PyStatus:
    cdef Status status  # Hold a C++ instance which we're wrapping

    def __cinit__(self):
        self.status = Status()

    def OK(self):
        return PyStatus()