from libcpp.string cimport string
from libcpp.vector cimport vector
cimport numpy as np
import numpy as np
cimport cython

cdef extern from "src/magnetar.h":
    cdef cppclass cMagnetar:
        cMagnetar(string)
        void setup(double,double,double,double,double)
        double flux(double, string)
        vector[double] flux(double*, size_t, string)

cdef class Magnetar:
    cdef cMagnetar *thisptr      # hold a C++ instance which we're wrapping

    def __cinit__(self, string folderPath):
        self.thisptr = new cMagnetar(folderPath)

    def __dealloc__(self):
        del self.thisptr

    def setup(self, double tau, double B, double P, double t0, double z):
        self.thisptr.setup(tau, B, P, t0, z)

    # def flux(self, double t, string f):
    #     return self.thisptr.flux(t, f)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def flux(self, np.ndarray[double, ndim=1, mode="c"] tArr not None, string f):
        return self.thisptr.flux(<double*> tArr.data, tArr.size, f)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def fluxPTF_R(self, np.ndarray[double, ndim=1, mode="c"] tArr not None):
        return self.thisptr.flux(<double*> tArr.data, tArr.size, "PTF48_R")

    def fluxPTF_R(self, double t):
        return self.thisptr.flux(t, "PTF48_R")
