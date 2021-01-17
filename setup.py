from setuptools import setup, Extension
redpower_module = Extension('redpower',
        sources = ['yespower.c'],
        extra_compile_args=['-march=native', '-funroll-loops', '-fomit-frame-pointer'],
        include_dirs=['.'])

setup (name = 'redpower',
       version = '1.0.1',
       description = 'Bindings for REDPOWER proof of work algorithm.',
       ext_modules = [redpower_module])
