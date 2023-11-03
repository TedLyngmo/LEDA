SHELL=/bin/sh

FLAGS = -O

all: 
	cd basic;     $(MAKE) -i CFLAGS=$(FLAGS)
	cd basic_alg; $(MAKE) -i CFLAGS=$(FLAGS)
	cd numbers;   $(MAKE) -i CFLAGS=$(FLAGS);\
		      if [ `arch` = sun4 ]; then $(MAKE) sparc; fi
	cd dict;      $(MAKE) -i CFLAGS=$(FLAGS)
	cd prio;      $(MAKE) -i CFLAGS=$(FLAGS)
	cd graph;     $(MAKE) -i CFLAGS=$(FLAGS)
	cd graph_alg; $(MAKE) -i CFLAGS=$(FLAGS)
	cd plane;     $(MAKE) -i CFLAGS=$(FLAGS)
	cd plane_alg; $(MAKE) -i CFLAGS=$(FLAGS)
	cd window;    $(MAKE) -i CFLAGS=$(FLAGS)
	cd x11;       $(MAKE) -i CFLAGS=$(FLAGS)

clean: 
	cd basic;     $(MAKE) -i clean
	cd basic_alg; $(MAKE) -i clean
	cd numbers;   $(MAKE) -i clean;\
		      if [ `arch` = sun4 ]; then $(MAKE) -i sparc-clean; fi
	cd dict;      $(MAKE) -i clean
	cd prio;      $(MAKE) -i clean
	cd graph;     $(MAKE) -i clean
	cd graph_alg; $(MAKE) -i clean
	cd plane;     $(MAKE) -i clean
	cd plane_alg; $(MAKE) -i clean
	cd window;    $(MAKE) -i clean
	cd x11;       $(MAKE) -i clean

