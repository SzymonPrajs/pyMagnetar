cdef extern from "magnetar.h":
    cdef cppclass cMagnetar:
        cMagnetar()
        int filterTest()
        
cdef class Magnetar:
    cdef cMagnetar *thisptr      # hold a C++ instance which we're wrapping
    def __cinit__(self):
        self.thisptr = new cMagnetar()
    def __dealloc__(self):
        del self.thisptr
    def filterTest(self):
        return self.thisptr.filterTest()
