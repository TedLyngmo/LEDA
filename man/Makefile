
MANUAL.ps: MANUAL.dvi
	dvips MANUAL.dvi

MANUAL.dvi: MANUAL.tex MANUAL.mac
	./extmandir
	latex MANUAL.tex
	latex MANUAL.tex

clean:
	rm -f extract/* MANUAL.dvi MANUAL.aux MANUAL.dvi MANUAL.log MANUAL.toc
