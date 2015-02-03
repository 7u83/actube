
OPENSSL_VERSION=openssl-1.0.1i
#OPENSSL_VERSION=openssl-1.0.1l
#OPENSSL_VERSION=openssl-1.0.2

ifeq ($(USE_CONTRIB_OPENSSL),1)
OPENSSLLIB=../contrib/$(OPENSSL_VERSION)/libssl.a ../contrib/$(OPENSSL_VERSION)/libcrypto.a -ldl
OPENSSLINC=../contrib/$(OPENSSL_VERSION)/include/openssl
else
OPENSSLLIB=-lssl
OPENSSLINC=openssl
endif


