all: fsDB

objs = main.o thread_pool.o producer_consumer.o
libs = -lpthread
cflags = -std=gnu99 -Werror

fsDB: $(objs)
	gcc $(objs) -o fsDB $(libs)

main.o: main.c 
	gcc $(cflags) -c main.c $(libs)

thread_pool.o: thread_pool.c
	gcc $(cflags) -c thread_pool.c $(libs)

producer_consumer.o: producer_consumer.c
	gcc $(cflags) -c producer_consumer.c $(libs)

clean:
	rm -rf *.o fsDB
