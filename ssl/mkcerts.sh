#!/bin/sh

rm index.txt
rm serial
echo '1000' > serial
touch index.txt

echo Creating Root CA 
openssl genrsa -des3 -out root-ca.key 2048
openssl req -new -x509 -days 3650 -key root-ca.key -out root-ca.crt -config openssl.cnf

echo Creating AC Req
openssl req -newkey rsa:1024 -keyout ac.key -config openssl.cnf -out ac.req
openssl ca -config openssl.cnf  -out ac.crt -infiles ac.req

echo Creating Wtp Cert
openssl req -newkey rsa:1024 -keyout wtp.key -config openssl.cnf -out wtp.req
openssl ca -config openssl.cnf -out wtp.crt -infiles wtp.req

