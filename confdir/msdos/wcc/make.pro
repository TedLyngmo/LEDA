#-----------------------------------------------------------------------------
# WATCOM
#-----------------------------------------------------------------------------
e = .exe

.obj.exe:
	wcl386 -zq -cc++ -x -k30000 $*.obj $(DOSLIB)
.c.obj:
	wcl386 -zq -cc++ -I..\\..\\incl -c $*.c
.c.exe:
	wcl386 -zq -cc++ -I..\\..\\incl -c $*.c
	wcl386 -zq -cc++ -x -k30000 $*.obj $(DOSLIB)


include Make.lst

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) core

