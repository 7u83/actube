#!/bin/sh
rm index.*
rm serial.*
echo '1000' > serial
touch index.txt

echo Creating AC Req 
openssl req -newkey rsa:1024 -keyout ac_cisco.key -config openssl-cisco.cnf -out ac_cisco.req
openssl ca -config openssl-cisco.cnf  -out ac_cisco.crt -infiles ac_cisco.req 
openssl x509 -in ac_cisco.crt -out ac_cisco.pem

# create certificate chain
cat cisco/cisco-ca.crt cisco/cisco-root-ca.crt >> ac_cisco.pem

#remove key from keyfile
openssl rsa -in ac_cisco.key -out ac_cisco.key


