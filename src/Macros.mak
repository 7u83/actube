ifeq (${USE_CONTRIB_OPENSSL},1)
OPENSSL_LIBS=../contrib/${OPENSSL_VERSION}/libssl.a ../contrib/${OPENSSL_VERSION}/libcrypto.a -ldl
OPENSSL_CFLAGS=../contrib/${OPENSSL_VERSION}/include/
else
OPENSSL_LDFLAGS=-lssl -lcrypto -ldl
OPENSSL_CFLAGS=
endif

ifeq ($(USE_CONTRIB_GNUTLS),1)
GNUTLS_CFLAGS=-I../contrib/gnutls-${GNUTLS_VERSION}/lib/includes
#GNUTLS_LIBS=-lnettle -lgmp ../contrib/gnutls-${GNUTLS_VERSION}/lib/.libs/libgnutls.a 
GNUTLS_LIBS=-lgmp -lgnutls -lnettle

GNUTLS_LDFLAGS=-L../contrib/gnutls-${GNUTLS_VERSION}/lib/.libs/ 
else
GNUTLS_CFLAGS=
GNUTLS_LIBS=-lgnutls -lnettle -lgmp
GNUTLS_LDFLAGS=
#-lgnutls -lnettle -lgmp

endif


ifndef ARCH
	ARCH = $(shell $(CC) -dumpmachine)
endif


