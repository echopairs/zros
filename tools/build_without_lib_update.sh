
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

EXTERNAL_LIBS_DIR=$route/../external_libs

# init splog, not good here
if [ ! -f "$route"/../external_libs/spdlog.tar.gz ]; then
  echo "---spdlog is not exit, git clone https://github.com/echopairs/spdlog.git" || exit -1
  fi

tar -zxvf "$EXTERNAL_LIBS_DIR"/spdlog.tar.gz -C /usr/local/include >/dev/null 2>&1|| exit -2

