#include <stdlib.h>
#include <stdio.h>
#include "net_support.h"
#include "producer_consumer.h"

producer_consumer prod_cons;
int server_socket;

void *
socket_producer(void *data)
{
  server_socket = tcp_server(8080);
  if (server_socket < 0) {
    printf("Failed to create socket\n");
    exit(1);
  }
  
  int connection;
  printf("Starting to listen\n");
  while (1) {
    connection = accept(server_socket, NULL,NULL);
    if (connection < 0) {
      printf("Accept error\n");
      exit(1);
    }
    int *number = (int *)malloc(sizeof(int));
    *number = connection;
    produce(&prod_cons, (void *)number);
  }
}

void *
request_handler(void *data)
{
  //TODO - GET DATA FROM FILE AND SEND DOWN LINE
}


int 
main(int argc, char **args) 
{
  //TODO - GET NUMBER OF THREADS TO RUN, PORT, SIZE OF ITEM, FILENAME AND MAX
  //TODO - SETUP PRODUCER_CONSUMER AND START SERVER
}

