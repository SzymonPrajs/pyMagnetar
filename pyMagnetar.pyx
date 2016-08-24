from libcpp.string cimport string

cdef extern from "src/magnetar.h":
    cdef cppclass cMagnetar:
        cMagnetar(string)
        void setup(double,double,double,double)
        float flux(double, string)

cdef class Magnetar:
    cdef cMagnetar *thisptr      # hold a C++ instance which we're wrapping

    def __cinit__(self, string folderPath):
        self.thisptr = new cMagnetar(folderPath)

    def __dealloc__(self):
        del self.thisptr

    def setup(self, double tau, double B, double P, double z):
        self.thisptr.setup(tau, B, P, z)

    def flux(self, double t, string f):
        return self.thisptr.flux(t, f)

    def fluxPTF_R(self, double t):
        return self.thisptr.flux(t, "PTF48_R")
