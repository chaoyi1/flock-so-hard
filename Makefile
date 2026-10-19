CFLAGS = -Wall -ggdb -std=c11 -pedantic -Iinclude -O2	
LIBS = -lraylib -lm -lX11 -lGL -ldl -pthread

flockin: src/main.c src/simulation.c behaviours/alignment.c behaviours/cohesion.c behaviours/separation.c behaviours/helper.c
	$(CC) $(CFLAGS) -o flockin src/main.c src/simulation.c behaviours/alignment.c behaviours/cohesion.c behaviours/separation.c behaviours/helper.c $(LIBS)