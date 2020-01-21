# distutils: language = c++

from libcpp.string cimport string

cdef extern from "Model.h" namespace "model":
    cdef cppclass Model:
        Model(string) except +
        void say()
