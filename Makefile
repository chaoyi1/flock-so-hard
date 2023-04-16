CFLAGS=-Wall -ggdb -std=c11 -pedantic
LIBS= -lraylib -lm
flockin: main.c
		 $(CC) $(CFLAGS) -o flockin main.c boid.c $(LIBS)
