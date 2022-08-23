#include ../Config.default.mak
#-include ../Config.mak
include ../Defs.mak

OBJS = \
	wtplist.o wtpman.o conf.o ac_main.o \
	dataman.o \
	socklist.o \
	discovery_cache.o\
	rpc.o\
	statemachine.o\

ACTOBJS = \
	act.o 

ACTPRG=act
PRG=actube
LIBPATH=-L../../lib/$(KERNEL)/$(ARCH)/ -L/usr/local/lib -L/usr/lib 

LIBS+=-lmavl
LIBS+=-lcw
LIBS+=-lrt
LIBS+=-lpthread
LIBS+=-lc
LIBS+=-lnettle
LIBS+=-lssl
LIBS+=-lcrypto
LIBS+=-ledit

INCL_DIRS=-I../ -I/usr/local/include -I./ -I../../include
#FLAGS=-DWITH_IPV6 -DWITH_OPENSSL -DSYS_ARCH="$(ARCH)" -DSYS_ARCH="XXX"
FLAGS=-DWITH_IPV6 -DUSE_OPENSSL -DSYS_ARCH='"$(ARCH)"'




.c.o:
	@echo "  $(CC) "$<
	$(CC) $(FLAGS) $(INCL_DIRS) -c $(CFLAGS) $< -o $@

all: $(PRG) $(ACTPRG)

$(PRG): $(OBJS) 
	$(CC) $(OBJS) -o $(PRG) $(LIBPATH) $(LDFLAGS) $(LIBS) 

$(ACTPRG): $(ACTOBJS)
	$(CC) $(ACTOBJS) -o $(ACTPRG) $(LIBPATH) $(LDFLAGS) -ledit $(LIBS) 





clean: 
	rm -f $(PRG) $(OBJS)
	rm -f $(ACTPRG) $(ACTOBJS)


