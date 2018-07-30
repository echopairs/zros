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

UBUNTU_FLAG=`cat /etc/issue | grep -i ubuntu >/dev/null 2>&1 && echo true || echo false`
CENTOS_FLAG=`cat /etc/issue | grep -i centos >/dev/null 2>&1 && echo true || echo false`

. ${route}/gen_all_external_lib.sh
cd ${route}/../external_libs/


if [ "$UBUNTU_FLAG"X = "true"X ]; then
        cd debs
        for deb in `ls *.deb`;
        do
            sudo dpkg -i $deb
        done
   else
        path=${route}/../external_libs/rpms/
        cd rpms
        for rpm in `ls *.rpm`;
        do
            sudo rpm -ivh $rpm
        done
fi