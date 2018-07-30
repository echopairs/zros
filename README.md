# zros

基于grpc/protobuf/zeromq的分布式网络框架
env: centos7.2/unbuntu
language: c++11/golang/python/node.js


## pre Requirements

### Tools:
    1. cmake (The main build tool, version >= 3.2)
    2. python 2.7.11 (refer to `src\jrpc_python` to get more info)
    3. nodejs 4.2.6 (refer to `src\jrpc_node` to get more info)

    More in Linux:
        1) run `sudo apt-get install build-essential autoconf libtool zlib1g-dev`
        2) run `sudo apt-get install checkinstall`

### External libs:
    Linux:
        1. grpc(last tag)
        2. protobuf
        3. gtest
        4. zmq
        5. zmq++
        6. sspdlog
        7. gperftools

        Use tool `tools\lib_update_posix.sh` to quickly fetch prebuilt  and automatically reinstall all libs.

## Build

### cpp:
    Linux:
        1. Run `tools\build_without_lib_update_posix.sh` to build automatically.

### python:

### node:

## example

## Folder Structure