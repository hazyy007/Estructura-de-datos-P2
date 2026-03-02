CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
# LDFLAGS indica que busque librerías en el directorio actual (.)
LDFLAGS = -L. 
# LDLIBS indica que use la librería 'stack' (libstack.a)
LDLIBS = -lstack

p2_e1: p2_e1.o music.o radio.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

p2_e1.o: p2_e1.c stack.h music.h radio.h
	$(CC) $(CFLAGS) -c p2_e1.c

music.o: music.c music.h types.h
	$(CC) $(CFLAGS) -c music.c

radio.o: radio.c radio.h music.h
	$(CC) $(CFLAGS) -c radio.c

clean:
	rm -f *.o p2_e1