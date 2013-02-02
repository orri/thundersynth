
CC=gcc
LIBS=-lasound -lm
CFLAGS=-g -Wall
all: link


compile:
	$(CC) -c channel.c thundersynth.c constantarrays.c $(CFLAGS)

link: compile
	$(CC) -o thundersynth channel.o thundersynth.o $(LIBS) 

clean:
	rm -f *.o thundersynth *~

