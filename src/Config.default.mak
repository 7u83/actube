# 
# OpenSSL definitions
# Chose either the contrib version or the system installed version
#

#OPENSSL_VERSION=openssl-1.0.1i
#OPENSSL_LIBS=../contrib/${OPENSSL_VERSION}/libssl.a ../contrib/${OPENSSL_VERSION}/libcrypto.a -ldl
#OPENSSL_CFLAGS=../contrib/${OPENSSL_VERSION}/include/

OPENSSL_LIBS=-lssl -lcrypto #-ldl
OPENSSL_CFLAGS=



# GnuTLS definitions
# GNUTLS_VERSION=3.3.9

USE_CONTRIB_GNUTLS=0


COMPDEFS=-DWITH_CW_LOG
COMPDEFS+=-DWITH_CW_LOG_DEBUG

#
# LIBCW settings

# WITH_GNUTLS
# compile in support for GnuTLS
#
WITH_GNUTLS=1

# 
# Compile in openssl support
#
WITH_OPENSSL=1

# 
# 
LIBDIR=lib 

OPTFLAGS := -g -O0
#CFLAGS	:=	-D_XOPEN_SOURCE=600 -D_BSD_SOURCE -D_DEFAULT_SOURCE=1 -pedantic -std=c99 -Wall





