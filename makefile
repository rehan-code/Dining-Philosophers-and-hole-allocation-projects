CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

all: dine holes

dine.o: dine.c
	$(CC) $(CFLAGS) -pthread -c dine.c -o dine.o

dine: dine.o dine.c
	$(CC) $(CFLAGS) -pthread -o dine dine.o

holes.o: holes.c
	$(CC) $(CFLAGS) -pthread -c holes.c -o holes.o

holes: holes.o holes.c
	$(CC) $(CFLAGS) -pthread -o holes holes.o

clean:
	rm *.o dine holes