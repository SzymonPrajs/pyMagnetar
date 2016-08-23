from libcpp.string cimport string

cdef extern from "src/magnetar.h":
    cdef cppclass cMagnetar:
        cMagnetar(string)
        int filterTest(string)

cdef class Magnetar:
    cdef cMagnetar *thisptr      # hold a C++ instance which we're wrapping
    def __cinit__(self, string folderPath):
        self.thisptr = new cMagnetar(folderPath)
    def __dealloc__(self):
        del self.thisptr
