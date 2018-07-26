#!/usr/bin/env bash

script=$(readlink -f "$0")
route=$(dirname "$script")

if [ -d "$route/install_zmq" ]; then
   sudo  rm -rf $route/install_zmq
fi

mkdir $route/install_zmq
cd $route/install_zmq

# build zmp
git clone https://github.com/zeromq/libzmq
cd libzmq && mkdir build
cd build
cmake ../ || exit 1
make -j 4 || exit 2
make test || exit 3
sudo checkinstall -D --install=no --fstrans=no --default --pkgname=zmq --pkgversion=4.2.2 --pakdir=$route || exit 4
# build zmqc++
cd $route/install_zmq
git clone https://github.com/zeromq/cppzmq.git
cd cppzmq && mkdir build
cd build
cmake ../ || exit 5
make -j 4 || exit 6
sudo checkinstall -D --install=no --fstrans=no --default --pkgname=zmq++ --pkgversion=4.2.1 --pakdir=$route