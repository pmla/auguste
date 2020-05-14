import os
import sys
import numpy
from setuptools import Extension, find_packages, setup
from distutils.sysconfig import get_config_vars
from distutils.version import LooseVersion
import platform


major_version = 0
minor_version = 1
subminor_version = 5
version = '%d.%d.%d' % (major_version, minor_version, subminor_version)

python_version = "%d.%d%s" % (sys.version_info.major,
                              sys.version_info.minor,
                              sys.abiflags)
extra_compile_args = []


def is_platform_mac():
    return sys.platform == "darwin"


# Build for at least macOS 10.9 when compiling on a 10.9 system or above,
# overriding CPython distuitls behaviour which is to target the version that
# python was built for. This may be overridden by setting
# MACOSX_DEPLOYMENT_TARGET before calling setup.py
if is_platform_mac():
    if "MACOSX_DEPLOYMENT_TARGET" not in os.environ:
        current_system = platform.mac_ver()[0]
        python_target = get_config_vars().get(
            "MACOSX_DEPLOYMENT_TARGET", current_system
        )
        if (
            LooseVersion(python_target) < "10.9"
            and LooseVersion(current_system) >= "10.9"
        ):
            os.environ["MACOSX_DEPLOYMENT_TARGET"] = "10.9"

    if sys.version_info[:2] == (3, 8):
        extra_compile_args.append("-Wno-error=deprecated-declarations")


# read the contents of README.md
this_directory = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

module = Extension(
    'auguste',
    sources=['src/eigendecomposition.cpp',
             'src/lup_decomposition.cpp',
             'src/mahalonobis_transform.cpp',
             'src/matrix_vector.cpp',
             'src/minkowski_reduction.cpp',
             'src/parse_string.cpp',
             'src/polar_decomposition.cpp',
             'src/quaternion.cpp',
             'src/sqp_newton_lagrange.cpp',
             'src/stepwise_iteration.cpp',
             'src/symmetrization.cpp',
             'src/unimodular_functions.cpp',
             'src/auguste_module.cpp'],
    include_dirs=[os.path.join(numpy.get_include(), 'numpy'),
                  'src'],
    extra_compile_args=extra_compile_args,
    language='c++'
)

setup(name='auguste',
      python_requires='>=3.5.0',
      ext_modules=[module],
      version=version,
      description='Minimum-strain symmetrization of Bravais lattices',
      author='P. M. Larsen',
      author_email='peter.mahler.larsen@gmail.com',
      url='https://github.com/pmla/auguste',
      long_description_content_type='text/markdown',
      long_description=long_description,
      install_requires=['numpy', 'scipy'],
      packages=find_packages(),
)
