#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int client_handshake(int *from_server) {
  
  int to_server;
  char buffer[100];

  // make pipe with name pid of client
  sprintf(buffer, "%d", getpid() );
  mkfifo(buffer, 0644);
  
  // connects the the WKP sends the name of the client pipe
  to_server = open("mario", O_WRONLY);
  *strchr(buffer, '\n') = 0;
  write( to_server, buffer, sizeof(buffer));
  
  // connects to the client pipe
  *from_server = open(buffer, O_RDONLY);
  remove(buffer);

  
  read( *from_server, buffer, sizeof(buffer));
  printf("<client> connection established: [%s]\n", buffer);

  return to_server;  
}


int main() {
  int to_server;
  int from_server;
  char buffer[100];

  to_server = client_handshake(&from_server);

  while(1) {
    printf("<client> enter stuff:");
    fgets(buffer, sizeof(buffer), stdin);
    write(to_server, buffer, sizeof(buffer));
    read(from_server, buffer, sizeof(buffer));
    printf("<client> received: [%s]", buffer);
    
  }

  close(to_server);
  close(from_server);

  return 0;
}
