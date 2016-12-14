all: prog

prog: prog.c
	g++ prog.c --openmp -o prog	

run:
	for i in `seq 1 8` ; \
	do \
		export OMP_NUM_THREADS="$$i"; \
		./prog; \
	done; \

sendb:
	scp prog.c edu-cmc-pod16-004@bluegene.hpc.cs.msu.ru:~/prog/prog.c

sendr:
	scp prog.c edu-cmc-pod16-004@regatta.cs.msu.su:~/prog/prog.c

.PHONY: run sendb sendr
