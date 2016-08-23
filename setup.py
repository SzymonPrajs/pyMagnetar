from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext = Extension(
    name = "pyMagnetar",
    sources = ["pyMagnetar.pyx", "src/magnetar.cpp", "src/utils.cpp", "src/Filters.cpp", "src/Cosmology.cpp"],
    include_dirs = ["./", "src" "src/vmath"],
    language = "c++",
    extra_compile_args = ["-std=c++11"],
    extra_link_args = ["-std=c++11"]

)

setup(
    ext_modules = [ext],
    cmdclass = {'build_ext': build_ext}
)
