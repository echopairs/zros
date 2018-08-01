# -*- coding: utf-8 -*-
""" Setup tool for zros python
"""

import setuptools
from setuptools import setup
import os
import sys
import subprocess
from distutils.command.clean import clean as _clean
from distutils.command.build_py import build_py as _build_py
from distutils.spawn import find_executable

# 1. def python which
def py_which(executable):
    if executable in os.environ and os.path.exists(os.environ[executable]):
        exec_path = os.environ[executable]
    else:
        exec_path = find_executable(executable)
    return exec_path

# pb gen codes refers to http://stackoverflow.com/a/27847119
# 2. gen pb
def generate_proto(source, output):
    """Invokes the Protocol Compiler to generate a _pb2.py from the given
    .proto file.  Does nothing if the output already exists and is newer than
    the input."""
    if(not os.path.exists(output) or
       os.path.exists(source) and
       os.path.getmtime(source) > os.path.getmtime(output)):
        print "Generating %s..." % output
    if not os.path.exists(source):
        sys.stderr.write(" Can't find required file: %s \n" % source)
        sys.exit(-1)
    protoc_path = py_which("protoc")
    if protoc_path is None:
        sys.stderr.write("Protocol buffers Compiler 'protoc' not installed or not found.\n")
        sys.exit(-1)
    protoc_command = [ protoc_path,
                      "-I" + os.path.split(source)[0],
                      "--python_out=" + output,
                      "--grpc_out=" + output,
                      "--plugin=protoc-gen-grpc=" + os.path.split(os.path.realpath(protoc_path))[0] + "/grpc_python_plugin",
                      source]
    print protoc_command
    if subprocess.call(protoc_command) != 0:
        sys.exit(-1)

# 3. list of all .proto files
proto_src = [('src/zros_proto/zros.proto', 'src/zros_python/zros/pb/'),
             ('src/zros_proto/test_message.proto', 'src/zros_python/zros/pb/'),
             ('src/zros_proto/test_service.proto', 'src/zros_python/zros/pb/')]

class build_py(_build_py):
    def run(self):
        for f in proto_src:
            generate_proto(f[0],f[1])
        _build_py.run(self)


class clean(_clean):
    def run(self):
        # Delete generated files in the code tree.
        for (dirpath, dirnames, filenames) in os.walk(os.path.split(os.path.realpath(__file__))[0]):
            for filename in filenames:
                filepath = os.path.join(dirpath,filename)
                if filepath.endswith("_pb2.py") or filepath.endswith(".pyc"):
                    os.remove(filepath)
        # _clean is an old-style class, so super() doesn't work.
        _clean.run(self)

# 4. set up
setup (
    name='zros',
    version='0.0.1',
    description='A grpc/zeromq based zros',
    author='zsy',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Programming Language :: Python :: 2.7',
        'Private :: Do Not Upload',
    ],
    install_requires=[
        'grpcio==1.0.0',
        'pyzmq==16.0.2',
    ],
     packages=['zros',
               'zros.pb',
    #           'zros.client',
    #           'zros.server'],
               ],
    package_dir={'zros': 'src/zros_python/zros'},
    cmdclass={'clean':clean, 'build_py': build_py},
)
