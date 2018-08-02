## Requirements

- Python 2.7.11
- gRPC python 1.0.0
- zmq  python 16.0.2

## Build

### Windows


### Linux (ubuntu 1604)

#### Build a Target Wheel

    1) 安装pip, setuptools, wheels：
        `sudo apt install python-pip`

    2) 安装virtualenv：
        `pip install virtualenv` (默认安装在~/.local/bin/)
        `sudo pip install virtualenv` (默认安装在/usr/local/bin/)

    3) 新建一个virtualenv：
        `virtualenv ~/py_xx_env`

    4) 进入virtualenv：
        `source ~/py_xx_env/bin/activate`

    5) cd到目标文件夹制作wheel包（py项目的根目录，其中应包含setup.py和requirements.txt）
        `cd ~/zros`
        `python setup.py bdist_wheel` (制作wheel包)

    6) 目标的virtualenv环境中安装zros wheel包：
        `pip install xxx.whl`

#### Build in Dev Mode

    1) - 4) 同上述Target Build中描述

    5) cd到目标文件夹进行Dev安装（py项目的根目录，其中应包含setup.py和requirements.txt）
        `cd ~/zros`
        `python setup.py develop`
        `python setup.py build_py`
        `pip list` (check if zros is installed successfully)

    6) use zros in your proj

## Examples

(Refer to an example in [zros_python_example](https://github.com/echopairs/zros/tree/master/examples/python))