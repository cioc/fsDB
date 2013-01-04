concur_dir = ../concurrency_lib/build

libs = -lpthread
cflags = -std=gnu99 -Werror -I ../concurrency_lib/src

src_path = ../src

all: lib fsDB.o
	gcc thread_pool.o producer_consumer.o net_support.o fsDB.o -o fsDB $(libs)
	
lib:
	$(MAKE) -C $(concur_dir)
	cp $(concur_dir)/thread_pool.o thread_pool.o
	cp $(concur_dir)/producer_consumer.o producer_consumer.o
	cp $(concur_dir)/net_support.o net_support.o

fsDB.o: $(src_path)/fsDB.c
	gcc $(cflags) -c  $(src_path)/fsDB.c	

clean:
	rm -f *.o fsDB
	$(MAKE) clean -C $(concur_dir) 
