all: fsDB

fsDB: main.o thread_pool.o
	gcc main.o thread_pool.o -o fsDB -lpthread

main.o: main.c 
	gcc -std=gnu99 -c main.c -lpthread

thread_pool.o: thread_pool.c
	gcc -std=gnu99 -c thread_pool.c -lpthread

clean:
	rm -rf *.o fsDB
