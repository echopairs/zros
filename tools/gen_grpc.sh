#!/bin/bash
#-------------------------------------
# Filename gen_grpc.sh
# Revision 1.0
# Data 2016/10/24
# Des Install GRPC
#-------------------------------------

script=$(readlink -f "$0")
route=$(dirname "$script")
HAS_CHECKINSTALL=`which checkinstall >/dev/null 2>&1 && echo true || echo false`
echo $HAS_CHECKINSTALL
if [ "$HAS_CHECKINSTALL"x=="false"x ]; then 
echo "--please install checkinstall first"
echo "-- down src : git clone http://checkinstall.izto.org/checkinstall.git"
exit -1;
fi

# init grpc


# init protobuf

#git clone -b $(curl -L http://grpc.io/release) https://github.com/grpc/grpc
#cd grpc
#git submodule update --init
#make
#make install
