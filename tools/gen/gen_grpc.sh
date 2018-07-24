#!/bin/bash
#-------------------------------------
# Filename gen_grpc.sh
# Revision 1.0
# Data 2016/10/24
# Des Install GRPC
# author: pairs
# email:736418319@qq.com
# env: ubuntu 16.4
#-------------------------------------

script=$(readlink -f "$0")
route=$(dirname "$script")

function check_package_is_exist() {
    if [ -f ${route}/../../external_libs/debs/grpc* ]; then
        echo "grpc deb already gen"
        exit 0
    fi

    if [ -f ${route}/../../external_libs/rpms/grpc* ]; then
        echo "grpc rpm already gen"
        exit 0
    fi
}

check_package_is_exist

HAS_CHECKINSTALL=`which checkinstall >/dev/null 2>&1 && echo true || echo false`

UBUNTU_FLAG=`cat /etc/issue | grep -i ubuntu >/dev/null 2>&1 && echo true || echo false`
CENTOS_FLAG=`cat /etc/issue | grep -i centos >/dev/null 2>&1 && echo true || echo false`


if [ "$HAS_CHECKINSTALL"x = "false"x ]; then
echo "--please install checkinstall first"
echo "-- down src : git clone http://checkinstall.izto.org/checkinstall.git"
exit -1;
fi

# init autoconf
if [ "$UBUNTU_FLAG"X = "true"X ]; then
    echo "-- this is ubuntu system"
    echo "--begin install autoconf"
    sudo apt-get install build-essential autoconf libtool >/dev/null 2>&1
else if [ "$CENTOS_FLAG"x = "true"x ];then
    echo "-- this is centos system"
    echo "--begin install autoconf"
    yum -y install build-essential autoconf libtool >/dev/null 2>&1
else
    echo "-- not supports system except centos and ubuntu"
fi
fi


# get source code if dir exsits just remove it.(the code may be incomplete)
[ -d "$route/grpc" ] && rm -rf grpc

git clone -b $(curl -L http://grpc.io/release) https://github.com/grpc/grpc

# update submodule
cd grpc
git submodule update --init

grpc_branch=`git branch`
grpc_version=`echo ${grpc_branch##*v}`

protobuf_branch=`cat .gitmodules | grep -n2 'https://github.com/google/protobuf.git' | grep branch`
protobuf_version=`echo ${protobuf_branch##*' '}`

# init grpc
make -j 2 || (echo "init grpc failed" exit)
## checkinstall upon system
if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=grpc --pkgversion=${grpc_version} --pakdir=$route/../../external_libs/debs || exit 2
else
    echo "/usr/local/lib64" >/etc/ld.so.conf.d/installwatch.conf
    ldconfig
    ln -s /usr/local/lib/installwatch.so /usr/local/lib64/installwatch.so
    yum install -y rpm-build
    yum install -y gcc rpm-build pcre-devel rpmdevtools
    checkinstall -R --install=no --fstrans=no --default --pkgname=grpc --pkgversion=${grpc_version} --pakdir=$route/../../external_libs/rpms || exit 2
fi

# init protobuf

cd third_party/protobuf
autoreconf -f -i -Wall,no-obsolete
./configure
make -j 2 || exit 3

if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=protobuf --pkgversion=${protobuf_version} --pakdir=$route/../../external_libs/debs || exit 4
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=protobuf --pkgversion=${protobuf_version} --pakdir=$route/../../external_libs/rpms || exit 4
fi

sudo rm -rf grpc/