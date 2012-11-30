all: fsDB

fsDB: main.o
	gcc main.o -o fsDB -lpthread

main.o: main.c
	gcc -std=gnu99 -c main.c -lpthread

clean:
	rm -rf *.o fsDB
