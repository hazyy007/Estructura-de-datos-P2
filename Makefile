CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

LDFLAGS = -L.
LDLIBS = -lstack

all: p2_e1 p2_e2a p2_e2b

p2_e1: p2_e1.o music.o radio.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

p2_e1.o: p2_e1.c stack.h music.h radio.h
	$(CC) $(CFLAGS) -c p2_e1.c

p2_e2a: p2_e2a.o music.o radio.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

p2_e2a.o: p2_e2a.c stack.h music.h radio.h
	$(CC) $(CFLAGS) -c p2_e2a.c

p2_e2b: p2_e2b.o music.o radio.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

p2_e2b.o: p2_e2b.c stack.h music.h radio.h
	$(CC) $(CFLAGS) -c p2_e2b.c

music.o: music.c music.h types.h
	$(CC) $(CFLAGS) -c music.c

radio.o: radio.c radio.h music.h
	$(CC) $(CFLAGS) -c radio.c

exe1: p2_e1
	./p2_e1 radio.txt

exe2a: p2_e2a
	./p2_e2a playlist1.txt playlist2.txt

exe2b: p2_e2b
	./p2_e2b playlist1.txt playlist2.txt

clean:
	rm -f *.o p2_e1 p2_e2a p2_e2b