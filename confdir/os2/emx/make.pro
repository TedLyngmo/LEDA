#-----------------------------------------------------------------------------
# OS2  EMX g++
#-----------------------------------------------------------------------------

.SUFFIXES:
.SUFFIXES: .pm .exe .0 .o .c

CFLAGS =

e=.exe

.c.o:
	gcc -xc++ $(CFLAGS) -I../../incl -c $*.c

.o.exe:
	gcc $(CFLAGS) $*.o $(UNIXLIB) -liostream

.o.pm:
	gcc $(CFLAGS) -o $*.pm $*.o $(UNIXLIB) -liostream
	emxbind -bpq -rleda.res /cc/emx/bin/emxl $*.pm


#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------

include Make.lst

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) core

