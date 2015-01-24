#!/bin/sh

KEYSIZE=2048


NAME=$1

PREF=$2
SUBJ="/C=DE/ST=Berlin/L=Berlin/O=Cauwersin/CN=7u83.cauwersin.com/emailAddress=7u83@mail.ru"
if [ $2 = "cisco" ] 
then
	PREF="$2-"
	SUBJ="/C=US/ST=California/L=San Jose/O=Cisco Virtual Wireless LAN Controller/CN=DEVICE-AC-TUBE/emailAddress=7u83@mail.ru"
fi



DIR=./certs
ROOT_CA_DIR=./root-ca
INT_CA_DIR=./intermediate-ca

echo $NAME

if [ ! -e $DIR ] 
then
	mkdir $DIR
fi


openssl req -nodes -newkey rsa:$KEYSIZE -keyout $DIR/$NAME.key -out $DIR/$NAME.req   \
        -subj "$SUBJ"

openssl ca -config openssl-int.cnf  \
	   -keyfile $INT_CA_DIR/${PREF}int-ca.key \
	   -cert $INT_CA_DIR/${PREF}int-ca.crt \
	   -batch \
	   -out $DIR/$NAME.crt -infiles $DIR/$NAME.req 
	

openssl x509 -in $DIR/$NAME.crt -out $DIR/$NAME.pem

cat $INT_CA_DIR/${PREF}int-ca.crt >> $DIR/$NAME.pem
cat $ROOT_CA_DIR/${PREF}root-ca.crt >> $DIR/$NAME.pem



