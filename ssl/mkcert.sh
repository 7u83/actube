#!/bin/sh

KEYSIZE=2048
#OPENSSL="../src/contrib/openssl-1.0.1i/apps/openssl"
OPENSSL="openssl"

set -x

NAME=$1
PREF=$2

#if [ ! -z $2 ]
#then
#	PREF=""
#fi


SUBJ="/C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=C1130-c80aa9cd7fa4/emailAddress=7u83@mail.ru"
if [ "$PREF" = "cisco" ] 
then
	PREF="$2-"
	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Virtual Wireless LAN Controller/CN=DEVICE-AC-TUBE/emailAddress=7u83@mail.ru"
fi

if [ "$PREF" = "simple" ] 
then
	PREF="$2"
	SUBJ="/C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=C1130-908d43460000/mailAddress=7u83@mail.ru"
fi

if [ "$PREF" = "cisco-ap" ]
then
	PREF="$2-"
#	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1130-f866f2a342fc/emailAddress=support@cisco.com"
#	SUBJ="/C=US/ST=California/L=San Jose/O=airespace Inc/CN=C1130-f866f2a342fc/emailAddress=support@airespace.com"

#	SUBJ="/ST=California/L=San Jose/C=US/O=Cisco Systems/CN=C1130-c80aa9cd7fa4/emailAddress=support@cisco.com"
	#SUBJ="/ST=California/L=San Jose/C=US/O=Cisco Systems/CN=C1130-c80aa9cd7fa4/emailAddress=support@cisco.com"
#	SUBJ="/C=US/ST=California/L=San Jose/O=airespace Inc/CN=C1130-f866f2a342fc/emailAddress=support@airespace.com"
	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1130-c80aa9cd7fa4/emailAddress=support@cisco.com"
#	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Systems/CN=C1200-c80aa9cd7fa4/emailAddress=support@cisco.com"


fi


DIR=./certs
ROOT_CA_DIR=./root-ca
INT_CA_DIR=./intermediate-ca

echo $NAME

if [ ! -e $DIR ] 
then
	mkdir $DIR
fi


$OPENSSL req -nodes -newkey rsa:$KEYSIZE -keyout $DIR/$NAME.key -out $DIR/$NAME.req   \
        -subj "$SUBJ"

if [ "$2" = "simple" ]
then
$OPENSSL ca -config openssl-simple.cnf  \
	   -keyfile $ROOT_CA_DIR/${PREF}-root-ca.key \
	   -cert $ROOT_CA_DIR/${PREF}-root-ca.crt \
	   -batch \
	   -out $DIR/$NAME.crt -infiles $DIR/$NAME.req 

$OPENSSL x509 -in $DIR/$NAME.crt -out $DIR/$NAME.pem

elif [ "$2" = "nocisco-ap" ]
then
$OPENSSL ca -config openssl-simple.cnf  \
	   -keyfile $ROOT_CA_DIR/${PREF}root-ca.key \
	   -cert $ROOT_CA_DIR/${PREF}root-ca.crt \
	   -batch \
	   -out $DIR/$NAME.crt -infiles $DIR/$NAME.req 

$OPENSSL x509 -in $DIR/$NAME.crt -out $DIR/$NAME.pem


else
$OPENSSL ca -config openssl-int.cnf  \
	   -keyfile $INT_CA_DIR/${PREF}int-ca.key \
	   -cert $INT_CA_DIR/${PREF}int-ca.crt \
	   -batch \
	   -out $DIR/$NAME.crt -infiles $DIR/$NAME.req 


$OPENSSL x509 -in $DIR/$NAME.crt -out $DIR/$NAME.pem
cat $INT_CA_DIR/${PREF}int-ca.crt >> $DIR/$NAME.pem
cat $ROOT_CA_DIR/${PREF}root-ca.crt >> $DIR/$NAME.pem
$OPENSSL x509 -in $INT_CA_DIR/${PREF}int-ca.crt -noout -sha1 -fingerprint

fi	





