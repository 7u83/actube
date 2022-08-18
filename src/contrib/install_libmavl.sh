#!/bin/sh

VERSION="1.2.0"

wget -O libmavl-${VERSION}.tgz ftp://ftp.planix.org/pub/planix/libmavl-${VERSION}.tgz
tar xzvf libmavl-${VERSION}.tgz
cd libmavl-${VERSION}
cp ../libmavl-LocalDefs.mak .
make install

