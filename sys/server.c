#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int server_handshake( int *from_client) {
  int to_client;
  char buffer[100];
  // creates well-known pipe
  mkfifo("mario", 0644);
  printf("<server> waiting on client\n");
  *from_client = open("mario", O_RDONLY);

  // removes the well-known pipe
  remove("mario");
  
  // waiting for client
  read(*from_client,buffer,sizeof(buffer));
  printf("<server> connection request: [%s]\n", buffer);

  // connects to the client's pipe
  to_client = open(buffer, O_WRONLY);
  strncpy( buffer, "its-a-me, mario!", sizeof(buffer));
  write(to_client, buffer, sizeof(buffer));
  
  return to_client;  
}


int main() {
  int to_client;
  int from_client;
  char buffer[100];

  to_client = server_handshake( &from_client );
  
  while(1) {
    read(from_client, buffer, sizeof(buffer));
    printf("<server> received [%s]\n", buffer);
    strncat(buffer, "purple monkey dishwasher", sizeof(buffer) - 1);   
    write(to_client,buffer, sizeof(buffer));
  }
  close(to_client);
  close(from_client);

  return 0;
  
}
