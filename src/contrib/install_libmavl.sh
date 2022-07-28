#!/bin/sh

VERSION="1.1.0a"

wget -O libmavl-${VERSION}.tgz ftp://ftp.planix.org/pub/planix/libmavl-${VERSION}.tgz
tar xzvf libmavl-${VERSION}.tgz
cd libmavl-${VERSION}
cp ../libmavl-LocalDefs.mak .
make install

