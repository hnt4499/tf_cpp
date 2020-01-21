# distutils: language = c++

from Model cimport Model
from libcpp.string cimport string

cdef class PyModel:
    cdef Model model  # Hold a C++ instance which we're wrapping

    def __cinit__(self, string msg):
        self.model = Model(msg)

    def say(self):
        self.model.say()