from libcpp.string cimport string

cdef extern from "Model.cpp":
    pass

cdef extern from "Model.h" namespace "model":
    cdef cppclass Model:
        Model() except +
        Model(string msg) except +
        void say()