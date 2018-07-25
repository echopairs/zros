#!/bin/bash

script=$(readlink -f "$0")
route=$(dirname "$script")

HAS_CHECKINSTALL=`which checkinstall >/dev/null 2>&1 && echo true || echo false`
UBUNTU_FLAG=`cat /etc/issue | grep -i ubuntu >/dev/null 2>&1 && echo true || echo false`
CENTOS_FLAG=`cat /etc/issue | grep -i centos >/dev/null 2>&1 && echo true || echo false`


function check_package_is_exist() {
        if [ -f ${route}/../../external_libs/debs/sspdlog* ]; then
            echo "grpc deb already gen"
            exit 0
        fi

        if [ -f ${route}/../../external_libs/rpms/sspdlog* ]; then
            echo "grpc rpm already gen"
            exit 0
        fi
}

check_package_is_exist

# get source code
[ -d "${route}/sspdlog" ] && rm -rf sspdlog

git clone https://github.com/zerbobo/sspdlog.git
cd sspdlog
version=`git rev-parse  --short HEAD`
mkdir build && cd build
cmake ../
make -j 2 || exit 3

if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=sspdlog --pkgversion=${version} --pakdir=$route/../../external_libs/debs || exit 4
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=protobuf --pkgversion=${version} --pakdir=$route/../../external_libs/rpms || exit 4
fi


