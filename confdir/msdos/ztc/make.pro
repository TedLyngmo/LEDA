#-----------------------------------------------------------------------------
# ZORTECH
#-----------------------------------------------------------------------------
e = .exe

.obj.exe:
	ztc -cpp -mx $*.obj $(DOSLIB)
.c.obj:
	ztc -cpp -mx -I..\..\incl -c $*.c
.c.exe:
	ztc -cpp -mx -I..\..\incl -c $*.c
	ztc -cpp -mx $*.obj $(DOSLIB)


include Make.lst

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) core


