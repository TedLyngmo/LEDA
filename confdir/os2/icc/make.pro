#-----------------------------------------------------------------------------
# OS2 IBM C++  
#-----------------------------------------------------------------------------

.SUFFIXES:
.SUFFIXES: .pm .exe .obj .c

CFLAGS =

e=.exe

.c.obj:
	icc -q -Tdp -I../../incl $(CFLAGS) $(DFLAGS) -c $*.c

.obj.exe:
	icc -q -Tdp $(CFLAGS) $*.obj $(DOSLIB)


.obj.pm:
	link386 /nologo /noe /noi $*.obj,$*.exe,,$(DOSLIB), leda.def;
	rc leda.res $*.exe

#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------

include Make.lst

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) core

