import os
import sys
import numpy
from distutils.core import setup, Extension

major_version = 0
minor_version = 1

python_version = "%d.%d%s" % (sys.version_info.major,
                              sys.version_info.minor,
                              sys.abiflags)

module1 = Extension('auguste',
                    define_macros = [('MAJOR_VERSION', str(major_version)),
                                     ('MINOR_VERSION', str(minor_version))],
                    include_dirs = [os.path.join(numpy.get_include(), 'numpy')],
                    libraries = [],
                    library_dirs = [],
                    extra_compile_args = ["-std=c++11"],
                    extra_link_args = ["-lm", "-lpython%s" % (python_version)],
                    sources = ['eigendecomposition.cpp',
                               'lup.cpp',
                               'mahalonobis_transform.cpp',
                               'matrix_vector.cpp',
                               'minkowski_reduction.cpp',
                               'parse_string.cpp',
                               'polar.cpp',
                               'quat.cpp',
                               'sqp_newton_lagrange.cpp',
                               'stepwise_iteration.cpp',
                               'symmetrization.cpp',
                               'unimodular_functions.cpp',
                               'auguste_module.c',
])

setup(name = 'auguste',
       version = '%d.%d' % (minor_version, minor_version),
       description = 'Minimum-strain Bravais lattice symmetrization',
       author = 'P. M. Larsen',
       author_email = 'pmla@mit.edu',
       url = 'https://github.com/pmla/auguste',
       long_description = '''
A python module for minimum-strain symmetrization of Bravais lattices.
The module can compute a distance from each Bravais lattice type, and
symmetrize an arbitrary lattice to a target Bravais type.''',
       ext_modules = [module1])
