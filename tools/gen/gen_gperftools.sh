#!/bin/bash

# get source code
[ -d "${route}/gperftools" ] && sudo rm -rf ${route}/gperftools

git clone https://github.com/gperftools/gperftools.git >/dev/null 2>&1
cd gperftools
last_tag=`git tag | tail -n1`
last_version=`git tag | tail -n1  | awk -F '-' '{print $2}'`
git checkout -b ${last_tag} >/dev/null 2>&1
git fetch origin ${last_tag} >/dev/null 2>&1
./autogen.sh >/dev/null 2>&1 || { echo "init autoreconf "; exit -2; }
./configure >/dev/null 2>&1
make -j 2 >/dev/null 2>&1

if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=gperftools --pkgversion=${last_version} --pakdir=$route/../external_libs/debs >/dev/null 2>&1|| exit 4
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=gperftools --pkgversion=${last_version} --pakdir=$route/../external_libs/rpms >/dev/null 2>&1 || exit 4
fi

printf "${GREEN}========== gen gperftools package success ===========\n"
sudo rm -rf ${route}/gperftools