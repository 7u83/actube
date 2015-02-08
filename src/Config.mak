
#Define SSL Library: OPENSSL or GNUTLS (GNUTLS not supported for now)
#SSL_LIBRARY=OPENSSL
SSL_LIBRARY=GNUTLS

# use openssl library in ../contrib/
OPENSSL_VERSION=openssl-1.0.1i
USE_CONTRIB_OPENSSL=1
#OPENSSL_VERSION=openssl-1.0.1l # doesn't work
#OPENSSL_VERSION=openssl-1.0.2	# doesn't work


# use gnutls lib in ../contrib/ 
GNUTLS_VERSION=3.3.9
USE_CONTRIB_GNUTLS=1

CONF_LIBRARY=UCI
USE_CONTRIB_UCI=0

CC=clang

#CC=mips-openwrt-linux-uclibc-gcc
#LD=mips-openwrt-linux-uclibc-ld
#AR=mips-openwrt-linux-uclibc-ar


