CFLAGS=-Wall -g -c -DDD_DEBUG
LFLAGS=-lm -g
#CFLAGS=-Wall -O2 -c -Wno-unused-parameter 
#LFLAGS=-lm -g -lddutil

SOURCE=codatabase.c \
coparse.c \
coscan.c \
main.c \
read.c

OBJS=$(patsubst %.c,%.o,$(SOURCE))

all: rpcgen

rpcgen: depends $(OBJS)
	gcc $(LFLAGS) -DDD_DEBUG -o rpcgen $(OBJS) -lddutil-dbg

depends:
	gcc -MM $(CFLAGS) $(SOURCE) > depends

clean:
	rm -f *.o codatabase.c codatabase.h rpcgen

codatabase.c: codatabase.h

codatabase.h: Command.dd
	datadraw Command.dd

coscan.c: coscan.l coparse.h co.h
	flex -f -i -Pco -o coscan.c coscan.l

coparse.c coparse.h: coparse.y co.h
	bison -d -p co -o coparse.c -b co coparse.y

-include depends