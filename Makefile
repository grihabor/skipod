all: prog

prog: prog.c
	g++ prog.c --openmp -o prog	

run:
	./prog

sendb:
	scp prog.c edu-cmc-pod16-004@bluegene.hpc.cs.msu.ru:~/prog/prog.c

sendr:
	scp prog.c edu-cmc-pod16-004@regatta.cs.msu.su:~/prog/prog.c

.PHONY: run sendb sendr
