#!/usr/bin/env bash

script=$(readlink -f "$0")
route=$(dirname "$script")

sudo apt install python-pip
sudo pip install virtualenv

virtualenv $route/py_tmp_env
source $route/py_tmp_env/bin/activate
cd $route/..

python setup.py bdist_wheel
cp $route/../dist/zros*.whl $route/
rm -rf dist zros.egg-info
deactivate
rm -rf $route/py_tmp_env
