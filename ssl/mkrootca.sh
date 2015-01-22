#!/bin/sh
echo Creating Root CA 

KEYSIZE=2048
CONFIG=openssl.cnf

ROOT_CA_DIR=./root-ca
INT_CA_DIR=./intermediate-ca

if [ ! -e $ROOT_CA_DIR ] 
then
	echo "Initializing root-ca"
	mkdir $ROOT_CA_DIR
	echo '1000' > $ROOT_CA_DIR/serial
	touch $ROOT_CA_DIR/index.txt
fi

openssl req -nodes -new -x509 \
	-sha1 \
	-extensions v3_ca \
	-days 3650 \
	-newkey rsa:2048 \
	-keyout $ROOT_CA_DIR/root-ca.key -out $ROOT_CA_DIR/root-ca.crt \
	-config openssl.cnf \
	-x509 \
        -subj /C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=7u83.cauwersin.com/emailAddress=7u83@mail.ru 



if [ ! -e $INT_CA_DIR ] 
then
	echo "Initializing intermediate-ca"
	mkdir $INT_CA_DIR
	echo '1000' > $INT_CA_DIR/serial
	touch $INT_CA_DIR/index.txt
fi

openssl genrsa -out $INT_CA_DIR/int-ca.key $KEYSIZE
openssl req -sha1 -new -key $INT_CA_DIR/int-ca.key -out $INT_CA_DIR/int-ca.csr \
        -subj /C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=7u83.cauwersin.com/emailAddress=7u83@mail.ru 


openssl ca -config openssl.cnf -keyfile $ROOT_CA_DIR/root-ca.key -cert $ROOT_CA_DIR/root-ca.crt \
	-extensions v3_ca -notext -md sha1 -in $INT_CA_DIR/int-ca.csr -out $INT_CA_DIR/int-ca.crt

