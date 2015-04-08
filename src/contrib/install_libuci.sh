#!/bin/sh

git clone git://nbd.name/luci2/libubox.git libubox
cd libubox
mkdir build
cd build
cmake ..
make ubox
cd ../..

git clone git://nbd.name/uci.git uci
cd uci
mkdir build
cd build

CFLAGS=-I../../
LDFLAGS=-L../../libubox/build/
export CFLAGS
export LDFLAGS

cmake ..
make uci cli
make install
