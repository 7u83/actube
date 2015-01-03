#!/bin/sh

wget https://www.openssl.org/source/openssl-1.0.1i.tar.gz
tar xzvf openssl-1.0.1i.tar.gz
patch -p0 -i openssl_cisco.patch
cd openssl-1.0.1i
./config
make


