from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

sourcefiles = ['pyPlusOne.pyx']
ext_modules = [Extension("pyPlusOne",
                          sourcefiles, language='c++',
                          )]

setup(
  name = 'test app',
  cmdclass = {'build_ext': build_ext},
  ext_modules = ext_modules
)