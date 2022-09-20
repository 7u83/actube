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
	#hapd.o

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
#LIBS+=-l:libhapd.a

INCL_DIRS=-I../ -I/usr/local/include -I./ -I../../include -I../../include/hostapd -I../../include/hostapd/utils
#FLAGS=-DWITH_IPV6 -DWITH_OPENSSL -DSYS_ARCH="$(ARCH)" -DSYS_ARCH="XXX"
FLAGS=-DWITH_IPV6 -DUSE_OPENSSL -DSYS_ARCH='"$(KERNEL)/$(ARCH)"'

all: act actube

act: act.c
	$(CC) act.c $(INCL_DIRS) $(LIBPATH) -DSYS_ARCH='"$(KERNEL)/$(ARCH)"' -o act -l:libcw.a -lasan -lcrypto -ledit

	

.c.o:
	@echo "  $(CC) "$<
	$(CC) $(FLAGS) $(INCL_DIRS) -c $(CFLAGS) $< -o $@

all: $(PRG) $(ACTPRG)

$(PRG): $(OBJS) 
	$(CC) $(OBJS) -o $(PRG) $(LIBPATH) $(LDFLAGS) $(LIBS) 

#$(ACTPRG): $(ACTOBJS)
#	$(CC) $(ACTOBJS) -o $(ACTPRG) $(LIBPATH) $(LIBS) -ledit  
	#$(CC) $(ACTOBJS) $(ACTPRG) $(LIBPATH) $(LDFLAGS) $(LIBS) -ledit  





clean: 
	rm -f $(PRG) $(OBJS)
	rm -f $(ACTPRG) $(ACTOBJS)


