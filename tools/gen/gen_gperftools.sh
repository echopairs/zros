#!/bin/bash

script=$(readlink -f "$0")
route=$(dirname "$script")

HAS_CHECKINSTALL=`which checkinstall >/dev/null 2>&1 && echo true || echo false`
UBUNTU_FLAG=`cat /etc/issue | grep -i ubuntu >/dev/null 2>&1 && echo true || echo false`
CENTOS_FLAG=`cat /etc/issue | grep -i centos >/dev/null 2>&1 && echo true || echo false`


function check_package_is_exist() {
        if [ -f ${route}/../../external_libs/debs/gperftools* ]; then
            echo "gperftools deb already gen"
            exit 0
        fi

        if [ -f ${route}/../../external_libs/rpms/gperftools* ]; then
            echo "gperftools rpm already gen"
            exit 0
        fi
}

check_package_is_exist

# get source code
[ -d "${route}/sspdlog" ] && rm -rf gperftools

git clone https://github.com/gperftools/gperftools.git
cd gperftools
version=`git rev-parse  --short HEAD`
./autogen.sh || { echo "init autoreconf "; exit -2; }
./configure
make -j 2

if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=gperftools --pkgversion=${version} --pakdir=$route/../../external_libs/debs || exit 4
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=gperftools --pkgversion=${version} --pakdir=$route/../../external_libs/rpms || exit 4
fi
