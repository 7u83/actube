#!/bin/sh

KEYSIZE=2048
#OPENSSL="../src/contrib/openssl-1.0.1i/apps/openssl"
OPENSSL="openssl"

set -x

NAME=$1
TYPE=$2



DIR=./certs
ROOT_CA_DIR=./root-ca
INT_CA_DIR=./intermediate-ca


if [ ! -e $DIR ] 
then
	mkdir $DIR
fi



createcert() 
{
	SUBJ=$1
	CNF=$2


	if [ ! -z $TYPE ]
	then
		PREF="$TYPE-"
	fi
	$OPENSSL genrsa -out $DIR/$NAME.key $KEYSIZE
	$OPENSSL req -sha1 -new -key $DIR/$NAME.key -out $DIR/$NAME.req   \
       		 -subj "$SUBJ"


	$OPENSSL ca -config openssl-int.cnf  \
		   -keyfile $INT_CA_DIR/${PREF}int-ca.key \
		   -cert $INT_CA_DIR/${PREF}int-ca.crt \
		   -batch \
	  	    -extensions v3_ca \
		   -out $DIR/$NAME.crt -infiles $DIR/$NAME.req 

	$OPENSSL x509 -in $DIR/$NAME.crt -out $DIR/$NAME.pem
	cat $INT_CA_DIR/${PREF}int-ca.crt >> $DIR/$NAME.pem
	cat $ROOT_CA_DIR/${PREF}root-ca.crt >> $DIR/$NAME.pem
	$OPENSSL x509 -in $INT_CA_DIR/${PREF}int-ca.crt -noout -sha1 -fingerprint

}


if [ -z $TYPE ]
then
	SUBJ="/C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=C1130-c80aa9cd7fa4/emailAddress=7u83@mail.ru"
	createcert $SUBJ
fi

if [ "$TYPE" = "cisco-ac" ] 
then
	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Virtual Wireless LAN Controller/CN=DEVICE-AC-TUBE/emailAddress=7u83@mail.ru"
	createcert $SUBJ
fi


if [ "$TYPE" = "cisco-ap" ]
then
	PREF="$2-"
#	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1130-f866f2a342fc/emailAddress=support@cisco.com"
#	SUBJ="/C=US/ST=California/L=San Jose/O=airespace Inc/CN=C1130-f866f2a342fc/emailAddress=support@airespace.com"

#	SUBJ="/ST=California/L=San Jose/C=US/O=Cisco Systems/CN=C1130-c80aa9cd7fa4/emailAddress=support@cisco.com"
	#SUBJ="/ST=California/L=San Jose/C=US/O=Cisco Systems/CN=C1130-c80aa9cd7fa4/emailAddress=support@cisco.com"
#	SUBJ="/C=US/ST=California/L=San Jose/O=airespace Inc/CN=C1130-f866f2a342fc/emailAddress=support@airespace.com"
#	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1200-c80aa9cd7fa4/emailAddress=support@cisco.com"
	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1130-c80aa9cd7fa4/emailAddress=support@cisco.com"
	createcert "$SUBJ"


fi




