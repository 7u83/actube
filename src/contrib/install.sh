#!/bin/sh

wget https://www.openssl.org/source/openssl-1.0.1i.tar.gz
tar xzvf openssl-1.0.1i.tar.gz
cd openssl-1.0.1i
./config
make


