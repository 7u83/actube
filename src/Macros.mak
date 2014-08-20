
ifeq ($(USE_CONTRIB_OPENSSL),1)
OPENSSLLIB=../contrib/openssl-1.0.1i/libssl.a ../contrib/openssl-1.0.1i/libcrypto.a -ldl
OPENSSLINC=../contrib/openssl-1.0.1i/include
else
OPENSSLLIB=-lssl
endif


