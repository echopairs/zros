#!/bin/bash
#-------------------------------------
# Filename: lib_update_posix.sh
# Revision: 1.0
# Data: 2016/10/30
# Des: Install external_libs
# author: pairs
# email:736418319@qq.com
# env centos 7.2
#-------------------------------------

script=$(readlink -f "$0")
route=$(dirname "$script")

EXTERNAL_LIBS_DIR=$route/../external_libs
LIBS_RPM=$EXTERNAL_LIBS_DIR/libs_rpm # save external_libs rpm Reduce project members compile time

HAS_CHECKINSTALL=`which checkinstall >/dev/null 2>&1 && echo true || echo false` # checkinstall

GRPC_VERSION=1.0.0
GPERTOOLS_VERSION=2.5.0
PROTOBUF_VERSION=3.0.0-GA
mkdir -p $EXTERNAL_LIBS_DIR/libs_rpm 

# 1. judge external_libs if exist && depends
if [ ! -d "$route/"../external_libs ]; then
  echo "---external_libs dir is not exist, please mkdir external_Libs and downlone libs" && exit -1
fi
## check checkinstall if exist
if [ "$HAS_CHECKINSTALL"x = "false"x ]; then
echo "--please install checkinstall first"
echo "-- down src : git clone http://checkinstall.izto.org/checkinstall.git"
exit -1;
fi

# 2. init gperftools
if [ ! -f "$route/"../external_libs/gperftools.tar.gz ]; then
  echo "--- gperftools is not exist , git clone https://github.com/gperftools/gperftools.git down it" && exit -2
fi

tar -zxvf "$EXTERNAL_LIBS_DIR"/gperftools.tar.gz -C $EXTERNAL_LIBS_DIR || exit -2
cd "$EXTERNAL_LIBS_DIR"/gperftools
## gen configure
./autogen.sh || { echo "init autoreconf "; exit -2; }
./configure
make -j 2

if [ -f "$LIBS_RPM"/gperftools-"$GPERTOOLS_VERSION"-1.x86_64.rpm ]; then
  rpm -qa | grep -q gperftools > /dev/null 1>&2 && rpm -e gperftools # remove old
  rpm -ivh "$LIBS_RPM"/gperftools-"$GPERTOOLS_VERSION"-1.x86_64.rpm || { echo "--rpm gperftools is failed"; exit -2; }
else
  checkinstall -R --install=yes --default --pkgname=gperftools --pkgversion=$GPERTOOLS_VERSION --pakdir=$LIBS_RPM --fstrans=no || { echo "build gperftools rpm is failed"; exit -2; }
fi
## rm the tmp
rm -rf "$EXTERNAL_LIBS_DIR"/gperftools
echo "-------------------init gperftools is success------------------------------"

# 3.1 init grpc && protobuf
if [ ! -f "$route"/../external_libs/grpc.tar.gz ]; then
  echo "---grpc is not exit, git clone -b $(curl -L http://grpc.io/release) https://github.com/grpc/grpc download to external_libs" || exit -3
fi

tar -zxvf "$EXTERNAL_LIBS_DIR"/grpc.tar.gz -C $EXTERNAL_LIBS_DIR || exit -3
## init depends
echo "--begin install autoconf"
yum -y install build-essential autoconf libtool >/dev/null 2>&1
cd "$EXTERNAL_LIBS_DIR"/grpc
git submodule update --init

# init grpc
make -j 2 || { echo "init grpc failed"; exit -3; }

if [ -f "$LIBS_RPM"/grpc-"$GRPC_VERSION"-1.x86_64.rpm ]; then
  rpm -qa | grep -q grpc > /dev/null 1>&2 && rpm -e grpc # remove old
  rpm -ivh "$LIBS_RPM"/grpc-"$GRPC_VERSION"-1.x86_64.rpm || exit -3
else
  echo "/usr/local/lib64" >/etc/ld.so.conf.d/installwatch.conf
  ldconfig
  ln -s /usr/local/lib/installwatch.so /usr/local/lib64/installwatch.so
  checkinstall -R --install=yes --fstrans=no --default --pkgname=grpc --pkgversion=$GRPC_VERSION  --pakdir=$LIBS_RPM --fstrans=no || { echo "build grpc rpm is faile"; exit -3; }
fi
echo "--------------------------------init grpc is success -------------------------"

### 3.2 init protobuf
cd "$EXTERNAL_LIBS_DIR"/grpc/third_party/protobuf
autoreconf -f -i -Wall,no-obsolete
./configure
make -j 2 || exit -3

if [ -f "$LIBS_RPM"/protobuf-"$PROTOBUF_VERSION"-1.x86_64.rpm ]; then
  rpm -qa | grep -q protobuf > /dev/null 1>&2 && rpm -e protobuf # remove old
  rpm -ivh "$LIBS_RPM"/protobuf-"$PROTOBUF_VERSION"-1.x86_64.rpm || { echo "--rpm gperftools is failed"; exit -2; }
else
  checkinstall -R --install=yes --default --pkgname=protobuf --pkgversion=$PROTOBUF_VERSION --pakdir=$LIBS_RPM --fstrans=no || { echo "build protobuf rpm is failed"; exit -3; }
fi
echo "------------------------------init protobuf is success----------------------"
rm -rf "$EXTERNAL_LIBS_DIR"/grpc
echo "---------------Congratulations on your libs update ok enjoy it ------------------ "
