
CC = ../../c++ -I../../incl

CFLAGS = -O

LIBS = ../../libP.a ../../libG.a ../../libL.a -lm
LIBX = ../../libP.a ../../libG.a ../../libL.a ../../libWx.a -lX11 -lm

.c.o:
	$(CC) $(CFLAGS) -c $*.c

all:  test diffi demo

sweep1.o: sweep0.tmp

sweep2.o: sweep0.tmp

sweep3.o: sweep1.tmp

sweep4.o: sweep1.tmp

test: test.o sweep0.o sweep1.o sweep2.o sweep3.o sweep4.o
	$(CC) $(CFLAGS) -o test test.o sweep?.o $(LIBS)

diffi: diffi.o sweep0.o sweep1.o sweep2.o
	$(CC) $(CFLAGS) -o diffi diffi.o sweep?.o $(LIBS)

demo: demo.o sweep0.o sweep1.o sweep2.o
	$(CC) $(CFLAGS) -o demo demo.o sweep?.o $(LIBX)

clean:
	rm -f *.o test demo diffi
