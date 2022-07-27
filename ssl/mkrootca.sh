#!/bin/sh
echo Creating Root CAs

KEYSIZE=2048
CONFIG=openssl.cnf

ROOT_CA_DIR=./root-ca
INT_CA_DIR=./intermediate-ca

DAYS=6000



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
	INT_SUBJ=$2
	NAME=$3


	if [ ! -z $NAME ] 
	then
		PREF="$NAME-"
	fi


	# Create a self-signed root CA
	openssl req -nodes -new -x509 \
		-sha1 \
		-days ${DAYS} \
		-extensions v3_ca \
		-newkey rsa:${KEYSIZE} \
		-keyout $ROOT_CA_DIR/${PREF}root-ca.key -out $ROOT_CA_DIR/${PREF}root-ca.crt \
		-config ${CONFIG} \
		-x509 \
		-subj "$ROOT_SUBJ"

	# Create a key for intermediate CA
	openssl genrsa -out $INT_CA_DIR/${PREF}int-ca.key $KEYSIZE

	# Create req for intermediate CA
	openssl req -sha1 -new -key $INT_CA_DIR/${PREF}int-ca.key -out $INT_CA_DIR/${PREF}int-ca.csr \
		-subj "$INT_SUBJ"

	# Sign intermediate CA cert using previously created root CA
	openssl ca -config ${CONFIG} -batch -keyfile $ROOT_CA_DIR/${PREF}root-ca.key \
		   -cert $ROOT_CA_DIR/${PREF}root-ca.crt \
		   -extensions v3_ca -notext -md sha1 -in $INT_CA_DIR/${PREF}int-ca.csr \
		   -out $INT_CA_DIR/${PREF}int-ca.crt



}


ROOT_SUBJ="/C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=7u83.cauwersin.com/emailAddress=7u83@mail.ru"
INT_SUBJ="$ROOT_SUBJ"
mkrootca "$ROOT_SUBJ" "$INT_SUBJ"


ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Virtual Wireless LAN Controller/CN=CA-vWLC-AIR-CTVM-K9-080027949DE0/emailAddress=support@vwlc.com"
INT_SUBJ="$ROOT_SUBJ"
mkrootca "$ROOT_SUBJ" "$INT_SUBJ" cisco-ac

#ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=airespace Inc/CN=C1130-f866f2a342fc/emailAddress=support@airespace.com"
#ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1130-f866f2a342fc/emailAddress=support@cisco.com"
#ROOT_SUBJ="/ST=California/L=San Jose/C=US/O=Cisco Systems/CN=C1130-f866f2a342fc/emailAddress=support@cisco.com"
#ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1200-c80aa9cd7fa4/emailAddress=support@cisco.com"
#ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=Cisrot/emailAddress=support@cisco.com"

#ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1130-c80aa9cd7fa4/emailAddress=support@cisco.com"
ROOT_SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1130-0019dbe09327/emailAddress=support@cisco.com"
INT_SUBJ="$ROOT_SUBJ"
mkrootca "$ROOT_SUBJ" "$INT_SUBJ" cisco-ap


