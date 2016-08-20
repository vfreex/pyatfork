#!/usr/bin/env python
import os
import sys
from distutils.log import warn as printf
from setuptools import setup, find_packages, Extension, Command

setup(
    name='pyatfork',
    version='0.1.1',
    description='PyAtfork provides API to register fork handlers.',
    license='MIT',
    author='Rayson Zhu',
    author_email='vfreex@gmail.com',
    packages = find_packages(),
    url='https://github.com/vfreex/pyatfork',
    ext_modules=[
        Extension(name='pyatfork.catfork',
            sources=['pyatfork/catfork_module/atfork.c',
                'pyatfork/catfork_module/catfork_module.c'],
            depends=['pyatfork/catfork_module/atfork.h'],
            ),
        ],
    long_description="PyAtfork is a wrapper for the POSIX Pthreads API pthread_atfork(3). "
    "I wrote this module because Python's ctype library cannot handle this function correctly.",

    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Libraries',
        'Intended Audience :: Information Technology',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.6',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Operating System :: POSIX',
        ],
    keywords='atfork pthread_atfork',
)

