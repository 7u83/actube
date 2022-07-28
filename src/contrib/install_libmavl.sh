#!/bin/sh

VERSION="1.1.0a"

wget ftp://ftp.planix.org/pub/planix/libmavl-${VERSION}.tgz
tar xzvf libmavl-${VERSION}.tgz
cd libmavl-${VERSION}
cp ../libmavl-LocalDefs.mak .
make install

