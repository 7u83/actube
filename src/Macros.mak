ifeq (${USE_CONTRIB_OPENSSL},1)
OPENSSL_LIBS=../contrib/${OPENSSL_VERSION}/libssl.a ../contrib/${OPENSSL_VERSION}/libcrypto.a -ldl
OPENSSL_CFLAGS=../contrib/${OPENSSL_VERSION}/include/
else
OPENSSL_LDFLAGS=
OPENSSL_CFLAGS=
OPENSSL_LIBS=-lssl -lcrypto #-ldl
endif

ifeq ($(USE_CONTRIB_GNUTLS),1)
GNUTLS_CFLAGS=-I../contrib/gnutls-${GNUTLS_VERSION}/lib/includes
GNUTLS_LIBS=-lgmp -lgnutls -lnettle
GNUTLS_LDFLAGS=-L../contrib/gnutls-${GNUTLS_VERSION}/lib/.libs/ 
else
GNUTLS_CFLAGS=
GNUTLS_LIBS=-lgnutls -lnettle -lgmp
GNUTLS_LDFLAGS=
endif


ifndef ARCH
	ARCH = $(shell uname -m)
endif


ifeq ($(CC),clang)
LDFLAGS		+= -g -L/usr/local/lib 
CFLAGS 		+= -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -fPIC -g -O0 -D_REENTRANT  -I /usr/local/include -I../
LD		= clang
endif

ifeq ($(CC),gcc)
LDFLAGS		+= -g -L/usr/local/lib 
CFLAGS 		+= -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -fPIC -g -O0 -D_REENTRANT  -I /usr/local/include -I../
LD		= gcc
endif

ifeq ($(CC),tcc)
LDFLAGS		+= -g -L/usr/local/lib 
CFLAGS 		+= -Wall -Wunusupported  -Wimplicit-function-declaration -I /usr/local/include -I../
LD		= tcc 
endif



CFLAGS		+= -DWITH_IPV6 -DWITH_RMAC_SUPPORT




