#!/usr/bin/env bash

if [ -d "$route/install_zmq" ]; then
   sudo  rm -rf $route/install_zmq
fi

mkdir $route/install_zmq
cd $route/install_zmq

# build zmp
git clone https://github.com/zeromq/libzmq >/dev/null 2>&1
cd libzmq && mkdir build
cd build
cmake ../ >/dev/null 2>&1 || exit 1
make -j 4 >/dev/null 2>&1 || exit 2
#make test || exit 3
if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=zmq --pkgversion=4.2.2 --pakdir=$route/../external_libs/debs >/dev/null 2>&1|| exit 4
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=zmq --pkgversion=4.2.2 --pakdir=$route/../external_libs/rpms >/dev/null 2>&1 || exit 4
fi
# build zmqc++
cd $route/install_zmq
git clone https://github.com/zeromq/cppzmq.git >/dev/null 2>&1
cd cppzmq && mkdir build
cd build
cmake ../ >/dev/null 2>&1 || exit 5
make -j 4 >/dev/null 2>&1 || exit 6

if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=zmq++ --pkgversion=4.2.2 --pakdir=$route/../external_libs/debs >/dev/null 2>&1|| exit 5
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=zmq++ --pkgversion=4.2.2 --pakdir=$route/../external_libs/rpms >/dev/null 2>&1 || exit 5
fi

printf "${GREEN}========== gen zmq package success ===========\n"
sudo  rm -rf $route/install_zmq