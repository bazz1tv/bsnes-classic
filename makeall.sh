#!/bin/bash
# :)
# designed for OS X
# Custom QT Path
export QT_PATH=/usr/local/Cellar/qt5/5.4.2
make -C bsnes
make -C snesfilter
make -C snesfilter install
make -C snesreader
make -C snesreader install
make -C supergameboy
make -C supergameboy install
