main: main.o rr.o fcfs.o sjf.o
	gcc -o main main.o rr.o fcfs.o sjf.o

main.o: main.c rr.h fcfs.h sjf.h
	gcc -c main.c

rr.o: rr.c rr.h
	gcc -c rr.c

fcfs.o: fcfs.c fcfs.h
	gcc -c fcfs.c

sjf.o: sjf.c sjf.h
	gcc -c sjf.c
