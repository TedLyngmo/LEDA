#-----------------------------------------------------------------------------
# DOS EMX g++
#-----------------------------------------------------------------------------

e = .emx

.o.emx:
	gcc -O -o $*.emx $*.o $(UNIXLIB) -liostream
.c.o:
	gcc -O -I../../incl -c -xc++ $*.c -xnone
.c.emx:
	gcc -O -I../../incl -c -xc++ $*.c -xnone
	gcc -O -o $*.emx $*.o $(UNIXLIB) -liostream


include Make.lst

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) core

