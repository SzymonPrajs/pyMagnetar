from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy

# Remove the "-Wstrict-prototypes" compiler option, which isn't valid for C++.
import distutils.sysconfig
cfg_vars = distutils.sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")

ext = Extension(
    name = "pyMagnetar",
    sources = ["pyMagnetar.pyx", "src/magnetar.cpp", "src/utils.cpp", "src/Filters.cpp", "src/Cosmology.cpp"],
    include_dirs = ["./", "src" "src/vmath", numpy.get_include()],
    libraries = ['gsl', 'cblas'],
    language = "c++",
    extra_compile_args = ["-std=c++11"],
    extra_link_args = ["-std=c++11"]

)

setup(
    ext_modules = [ext],
    cmdclass = {'build_ext': build_ext}
)
