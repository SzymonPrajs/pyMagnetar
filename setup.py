from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy
import sys

# Remove the "-Wstrict-prototypes" compiler option, which isn't valid for C++.
import distutils.sysconfig
cfg_vars = distutils.sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")


compile_args = ['-O3', '-std=c++11', '-stdlib=libc++']
link_args = ['-std=c++11', '-stdlib=libc++']
inc = ["./", "src" "src/vmath", numpy.get_include(), ".", "/usr/local/include",
       "/usr/local/include/"]
lib = [".", "/usr/local/lib", "/usr/local/lib/"]

if sys.platform == 'darwin':
    compile_args.append('-mmacosx-version-min=10.7')
    link_args.append('-mmacosx-version-min=10.7')

ext = Extension(
    name="pyMagnetar",
    sources=["pyMagnetar.pyx", "src/magnetar.cpp", "src/utils.cpp",
             "src/Filters.cpp", "src/Cosmology.cpp"],
    include_dirs=inc,
    libraries=['gsl', 'cblas'],
    language="c++",
    extra_compile_args=compile_args,
    extra_link_args=link_args

)

setup(
    ext_modules = [ext],
    cmdclass = {'build_ext': build_ext}
)
