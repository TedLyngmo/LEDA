
L = libL

include ../Make.src

 
sparc: $(OBJECTS) _sparc_mul.o _sparc_div.o _sparc_add.o _sparc_sub.o
	$(ARLIB)
	$(RANLIB)
	touch sparc

sparc_gnu:
	gcc -O -S _sgnu_add.c
	gcc -O -S _sgnu_sub.c
 
sparc-clean:
	rm -f _sparc*.o

