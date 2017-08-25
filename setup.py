#!/usr/bin/env python

from distutils.core import setup

setup(name='desk_cli',
      version='1.0',
      description='desk cli',
      author='Peter Mitrano',
      author_email='mitranopeter@gmail.com',
      url='https://github.com/petermitrano/desk/desk_cli',
      packages=['desk_cli'],
      scripts = ['desk_cli/desk'],
     )
