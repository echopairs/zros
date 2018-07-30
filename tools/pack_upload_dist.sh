#!/usr/bin/env bash


set -e
script=$(readlink -f "$0")
route=$(dirname "$script")
VERSION=`cat $route/../version`

#!/bin/bash
set -e
script=$(readlink -f "$0")
route=$(dirname "$script")
VERSION=`cat $route/../version`
which cmake >/dev/null 2>&1 || { echo "build requires cmake but nothing is found"; exit 1; }
if [ -d "$route/../build" ]; then
   sudo rm -rf $route/../build
fi
mkdir -p $route/../build || exit 2
cd $route/../build
cmake ../
make -j 2
sudo checkinstall -D --install=no --fstrans=no --default --pkgname=zros --pkgversion=$VERSION --pakdir=$route || exit 3