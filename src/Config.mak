# 
# OpenSSL definitions
#
# OPENSSL_VERSION=openssl-1.0.1i
USE_CONTRIB_OPENSSL=0

# GnuTLS definitions
# GNUTLS_VERSION=3.3.9

USE_CONTRIB_GNUTLS=0

# Compiler to use
# CC=clang
# CC=mips-openwrt-linux-uclibc-gcc
# LD=mips-openwrt-linux-uclibc-ld
# AR=mips-openwrt-linux-uclibc-ar
# CC=clang

COMPDEFS=-DWITH_CW_LOG
COMPDEFS+=-DWITH_CW_LOG_DEBUG


# LIBCW settings

# WITH_GNUTLS
# compile in support for GnuTLS
#
WITH_GNUTLS=1

# Compile in openssl support
WITH_OPENSSL=1

# 
# 
LIBDIR=lib 

 
