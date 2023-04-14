
CC=gcc
LIBS=-lasound -lm -lpthread
CFLAGS=-g -Wall
PROFILEFLAGS= -pg -fprofile-arcs -ftest-coverage
RELEASEFLAGS=-O3

CFILES=channel.c thundersynth.c read_input.c
OFILES=channel.o thundersynth.o read_input.o 

all: link

compile:
	$(CC) -c $(CFILES) $(CFLAGS)

link: compile
	$(CC) -o thundersynth $(OFILES) $(LIBS) 

profilecompile:
	$(CC) -c $(CFILES) $(CFLAGS) $(PROFILEFLAGS)

profilelink: profilecompile
	$(CC) -o thundersynth $(OFILES) $(LIBS) $(PROFILEFLAGS)

profile: profilelink
	./thundersynth; gprof -b ./thundersynth

clean:
	rm -f *.o thundersynth *~ gmon.out *.gcda *.gcno

