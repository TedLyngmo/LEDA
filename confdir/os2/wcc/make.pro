#-----------------------------------------------------------------------------
# OS2  WATCOM C++
#-----------------------------------------------------------------------------

.SUFFIXES:
.SUFFIXES: .pm .exe .obj .c 

e = .exe

.c.obj:
	wcl386 -zq -cc++ -I..\\..\\incl -c $*.c

.obj.pm:
	wcl386 -zq -x -k32k -bt=os2v2 -los2v2_pm $*.obj $(DOSLIB)
	rc leda.res $*.exe

.obj.exe:
	wcl386 -zq -x -k32k $*.obj $(DOSLIB)


!include Make.lst


all: $(PROGS)

clean:
	rm -f *.o $(PROGS) core

