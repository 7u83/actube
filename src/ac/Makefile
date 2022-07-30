#include ../Config.default.mak
#-include ../Config.mak
include ../Defs.mak

OBJS = \
	wtplist.o wtpman.o conf.o ac_main.o \
	dataman.o \
	ac_interface.o \
	socklist.o \
	db.o \
	ac_global.o \
	discovery_cache.o\
	shell.o\

PRG=actube
LIBPATH=-L../../lib/Linux/$(ARCH)/ -L/usr/local/lib -L/usr/lib 

LIBS+=-lmavl
LIBS+=-lcw
LIBS+=-lrt
LIBS+=-lpthread
LIBS+=-lconfuse
LIBS+=-lsqlite3
LIBS+=-lc
LIBS+=-lnettle
LIBS+=-lssl
LIBS+=$(OPENSSL_LIBS)

INCL_DIRS=-I../ -I/usr/local/include -I./ -I../../include
#FLAGS=-DWITH_IPV6 -DWITH_OPENSSL -DSYS_ARCH="$(ARCH)" -DSYS_ARCH="XXX"
FLAGS=-DWITH_IPV6 -DUSE_OPENSSL -DSYS_ARCH='"$(ARCH)"'

.c.o:
	@echo "  $(CC) "$<
	$(CC) $(FLAGS) $(INCL_DIRS) -c $(CFLAGS) $< -o $@


$(PRG): $(OBJS) 
#	$(CC) $(AC_OBJS) ../mod/modload_ac.o -o $(AC_NAME) $(LDFLAGS) $(LIBS)
	$(CC) $(LIBS) $(OBJS) -o $(PRG) $(LIBPATH) $(LDFLAGS) $(LIBS) 


clean: 
	rm  -f $(PRG) $(OBJS)


