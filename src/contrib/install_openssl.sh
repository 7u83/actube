#!/bin/sh

VERSION="1.0.1i"
#VERSION="1.0.2"

if [ !-e openssl-${VERSION} ]
then
wget https://www.openssl.org/source/openssl-${VERSION}.tar.gz
fi

tar xzvf openssl-${VERSION}.tar.gz
patch -p0 -i openssl-${VERSION}_cisco.patch
cd openssl-${VERSION}
./config
make


