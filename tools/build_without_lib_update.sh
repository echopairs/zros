
#!/bin/bash
#-------------------------------------
# Filename: lib_update_posix.sh
# Revision: 1.0
# Data: 2016/11/14
# Des: build 
# author: pairs
# email:736418319@qq.com
# env centos 7.2
#-------------------------------------

script=$(readlink -f "$0")
route=$(dirname "$script")

which cmake >/dev/null 2>&1 || { echo "build requires cmake but nothig is found"; exit 1; }

if [ -d "$route/../build" ]; then
    rm -rf $route/../build
fi

mkdir -p $route/../build

cd $route/../build
cmake .. -G "Unix Makefiles"
make -j 1



