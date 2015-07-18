#!/bin/bash
# :)
# designed for OS X
# Custom QT Path
export QT_PATH=/usr/local/Cellar/qt/4.8.7
make -C bsnes
make -C snesfilter
make -C snesfilter install
make -C snesreader
make -C snesreader install
make -C supergameboy
make -C supergameboy install