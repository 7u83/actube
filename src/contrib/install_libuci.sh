#!/bin/sh

git clone git://nbd.name/luci2/libubox.git libubox
cd libubox
mkdir build
cd build
cmake ..
make ubox
mkdir -p /usr/local/include/libubox
cp ../*.h /usr/local/include/libubox
cp libubox.so /usr/local/lib
ldconfig
cd ../..


git clone git://nbd.name/uci.git uci
cd uci
mkdir build
cd build

CFLAGS=-I../../
LDFLAGS=-L../../libubox/build/
export CFLAGS
export LDFLAGS

cmake .. -DBUILD_LUA:BOOL=OFF
make uci cli
make install
ldconfig
