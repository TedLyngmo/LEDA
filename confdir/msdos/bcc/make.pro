#-----------------------------------------------------------------------------
# BORLAND
#-----------------------------------------------------------------------------

.SWAP

e = .exe

.obj.exe:
	bcc -P -Qx -mh $*.obj $(DOSLIB)
.c.obj:
	bcc -P -Qx -w- -mh -I..\..\incl -c $*.c
.c.exe:
	bcc -P -Qx -w- -mh -I..\..\incl -c $*.c
	bcc -P -Qx -mh $*.obj $(DOSLIB)


!include "Make.lst"


all: $(PROGS)

clean:
	rm -f *.o $(PROGS) core


