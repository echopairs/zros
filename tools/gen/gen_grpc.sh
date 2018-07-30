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
[ -d "$route/grpc" ] && sudo rm -rf $route/grpc

git clone -b $(curl -L http://grpc.io/release) https://github.com/grpc/grpc >/dev/null 2>&1

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
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=grpc --pkgversion=${grpc_version} --pakdir=$route/../../external_libs/debs >/dev/null 2>&1 || exit 2
else
    echo "/usr/local/lib64" >/etc/ld.so.conf.d/installwatch.conf
    ldconfig
    ln -s /usr/local/lib/installwatch.so /usr/local/lib64/installwatch.so
    yum install -y rpm-build
    yum install -y gcc rpm-build pcre-devel rpmdevtools
    checkinstall -R --install=no --fstrans=no --default --pkgname=grpc --pkgversion=${grpc_version} --pakdir=$route/../../external_libs/rpms >/dev/null 2>&1 || exit 2
fi

# init protobuf

cd third_party/protobuf
autoreconf -f -i -Wall,no-obsolete
./configure
make -j 2 >/dev/null 2>&1 || exit 3

if [ "$UBUNTU_FLAG"X = "true"X ]; then
    sudo checkinstall -D --install=no --fstrans=no --default --pkgname=protobuf --pkgversion=${protobuf_version} --pakdir=$route/../external_libs/debs >/dev/null 2>&1|| exit 4
else
    /usr/local/sbin/checkinstall -D --install=no --fstrans=no --default --pkgname=protobuf --pkgversion=${protobuf_version} --pakdir=$route/../external_libs/rpms >/dev/null 2>&1 || exit 4
fi

printf "${GREEN}========== gen grpc&& protobuf package success ===========\n"
sudo rm -rf $route/grpc

