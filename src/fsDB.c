#include <stdlib.h>
#include <stdio.h>
#include "net_support.h"
#include "producer_consumer.h"

producer_consumer prod_cons;
int server_socket;

void *
socket_producer(void *data)
{
  //TODO - IMPLEMENT network server
}

void *
request_handler(void *data)
{
  //TODO - GET DATA FROM FILE AND SEND DOWN LINE
}


int 
main(int argc, char **args) 
{
  //TODO - GET NUMBER OF THREADS TO RUN, PORT, SIZE OF ITEM, AND MAX
  //TODO - SETUP PRODUCER_CONSUMER AND START SERVER
}

