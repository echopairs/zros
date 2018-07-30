#!/bin/bash

# get source code
[ -d "${route}/sspdlog" ] && sudo rm -rf ${route}/sspdlog

git clone https://github.com/zerbobo/sspdlog.git >/dev/null 2>&1
cd sspdlog
last_tag=`git tag | tail -n1`
last_version=`echo ${last_tag:1}`
git checkout -b ${last_tag} >/dev/null 2>&1
git fetch origin ${last_tag} >/dev/null 2>&1
mkdir build && cd build
cmake ../ >/dev/null 2>&1
make -j 2 >/dev/null 2>&1 || exit 3

if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=sspdlog --pkgversion=${last_version} --pakdir=$route/../external_libs/debs >/dev/null 2>&1 || exit 4
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=protobuf --pkgversion=${last_version} --pakdir=$route/../external_libs/rpms >/dev/null 2>&1|| exit 4
fi

printf "${GREEN}========== gen sspdlog package success ===========\n"
sudo rm -rf ${route}/sspdlog