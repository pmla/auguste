import os
import sys
import numpy
from setuptools import setup, Extension

major_version = 0
minor_version = 1
subminor_version = 3

python_version = "%d.%d%s" % (sys.version_info.major,
                              sys.version_info.minor,
                              sys.abiflags)


# read the contents of README.md
this_directory = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()


module = Extension('auguste',
                   define_macros = [('MAJOR_VERSION', str(major_version)),
                                    ('MINOR_VERSION', str(minor_version))],
                   include_dirs = [os.path.join(numpy.get_include(), 'numpy')],
                   libraries = [],
                   library_dirs = [],
                   sources = ['src/eigendecomposition.cpp',
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
                              'src/auguste_module.cpp',
])

setup(name = 'auguste',
      python_requires='>=3.5.0',
      ext_modules = [module],
      version = '%d.%d.%d' % (major_version, minor_version, subminor_version),
      description = 'Minimum-strain symmetrization of Bravais lattices',
      author = 'P. M. Larsen',
      author_email = 'pmla@fysik.dtu.dk',
      url = 'https://github.com/pmla/auguste',
      long_description_content_type='text/markdown',
      long_description=long_description,
      install_requires=['numpy',
                        'scipy'],
)
