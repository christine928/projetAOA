CC=gcc
CFLAGS=-O2 -g -Wall
OPTFLAGS=-O2 -g -Wall
OBJS=driverMediane.o kernel.o rdtsc.o

all:	baseline

baseline:	$(OBJS)
	$(CC) -o $@ $^ -lm -fopenmp

kernel.o: kernel.c projet.h
	$(CC) $(OPTFLAGS) -D $(OPT) -c $< -o $@ -lm -fopenmp

clean:
	rm -rf $(OBJS) baseline
