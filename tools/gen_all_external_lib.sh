#!/usr/bin/env bash

set -e
script=$(readlink -f "$0")
route=$(dirname "$script")

RED='\e[1;31m'
GREEN='\e[1;32m'
BLUE='\e[1;34m'
CYAN='\e[1;36m'
NC='\e[0m'

HAS_CHECKINSTALL=`which checkinstall >/dev/null 2>&1 && echo true || echo false`
UBUNTU_FLAG=`cat /etc/issue | grep -i ubuntu >/dev/null 2>&1 && echo true || echo false`
CENTOS_FLAG=`cat /etc/issue | grep -i centos >/dev/null 2>&1 && echo true || echo false`

declare -A all_packages
not_existing_packages=()

function gen_all_packages_dir() {
    printf "${BLUE}Begin gen_all_packages_dir \n"
    cd ${route}/gen
    for sh in `ls gen_*.sh`
    do
        length=`echo ${sh} | wc -L`
        package_name=`echo ${sh:4:length-7}`
        all_packages["${sh}"]=${package_name}
    done
    cd - >/dev/null 2>&1
}

# only support centos and ubuntu
function gen_not_existing_list() {
   printf "${BLUE}Begin gen_not_existing_list \n"
   if [ "$UBUNTU_FLAG"X = "true"X ]; then
        path=${route}/../external_libs/debs/
   else
        path=${route}/../external_libs/rpms/
   fi

   for key in ${!all_packages[*]}; do
        pn=${path}${all_packages[$key]}*
        if [ -f ${pn} ];then
            printf "${GREEN}${all_packages[$key]} package already gen \n"
        else
            package=`echo ${key}`
            not_existing_packages+=(
                ${package}
            )
        fi
   done
}

cd ${route}
gen_all_packages_dir
gen_not_existing_list

for sh in ${not_existing_packages[@]}; do
    printf "${BLUE}begin run ${sh} \n"
    printf "${NC}"
    source gen/${sh}
    cd ${route}
done