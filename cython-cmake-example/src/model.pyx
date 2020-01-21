# distutils: language = c++

from c_model cimport Model as CModel

cdef class Model:
    def __cinit__(self, str msg):
        self.model_ptr = new CModel(msg.encode("utf-8"))

    def __dealloc__(self):
        del self.model_ptr

    cpdef say(self):
        self.model_ptr.say()
