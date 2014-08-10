#!/bin/sh
rm index.*
rm serial.*
echo '1000' > serial
touch index.txt

echo Creating AC Req
openssl req -newkey rsa:1024 -keyout ac.key -config openssl.cnf -out ac.req
openssl ca -config openssl.cnf  -out ac.crt -infiles ac.req

echo Creating Wtp Cert
openssl req -newkey rsa:1024 -keyout wtp.key -config openssl.cnf -out wtp.req
openssl ca -config openssl.cnf -out wtp.crt -infiles wtp.req

