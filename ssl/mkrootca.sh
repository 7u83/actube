#!/bin/sh
echo Creating Root CA 
openssl genrsa -des3 -out root-ca.key 1536 -config openssl.cnf
openssl req -new -x509 -days 3365 -key root-ca.key -out root-ca.crt -config openssl.cnf 


