#!/bin/sh
echo Creating Root CAs

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

if [ ! -e $INT_CA_DIR ] 
then
	echo "Initializing intermediate-ca"
	mkdir $INT_CA_DIR
	echo '1000' > $INT_CA_DIR/serial
	touch $INT_CA_DIR/index.txt
fi


mkrootca() 
{
	ROOT_SUBJ=$1

	INT_SUBJ=$ROOT_SUBJ

	if [ ! -z $2 ] 
	then
		PREF="$2-"
	fi


	openssl req -nodes -new -x509 \
		-sha1 \
		-extensions v3_ca \
		-days 3650 \
		-newkey rsa:2048 \
		-keyout $ROOT_CA_DIR/${PREF}root-ca.key -out $ROOT_CA_DIR/${PREF}root-ca.crt \
		-config openssl.cnf \
		-x509 \
		-subj "$ROOT_SUBJ"

	openssl genrsa -out $INT_CA_DIR/${PREF}int-ca.key $KEYSIZE

	openssl req -sha1 -new -key $INT_CA_DIR/${PREF}int-ca.key -out $INT_CA_DIR/${PREF}int-ca.csr \
		-subj "$INT_SUBJ"

	openssl ca -config openssl.cnf -batch -keyfile $ROOT_CA_DIR/${PREF}root-ca.key \
		   -cert $ROOT_CA_DIR/${PREF}root-ca.crt \
		   -extensions v3_ca -notext -md sha1 -in $INT_CA_DIR/${PREF}int-ca.csr \
		   -out $INT_CA_DIR/${PREF}int-ca.crt


}


ROOT_SUBJ="/C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=7u83.cauwersin.com/emailAddress=7u83@mail.ru"
mkrootca "$ROOT_SUBJ" 

ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Virtual Wireless LAN Controller/CN=CA-vWLC-AIR-CTVM-K9-080027949DE0/emailAddress=support@vwlc.com"
mkrootca "$ROOT_SUBJ" cisco

