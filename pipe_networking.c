#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int from_client = 0;
  int dtlength;
  char *buffer;
  
  mkfifo(WKP,0600);

  int w_k_p = open(WKP,O_RDONLY);
  read(w_k_p,&dtlength,sizeof(int));
  buffer = malloc(dtlength);
  read(w_k_p,buffer,dtlength);

  from_client  = open(buffer,O_WRONLY);

  close(w_k_p);
  
  
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server = 0;
  char pidstring[12];
  int dtlength;
  
  int w_k_p = open(WKP,O_WRONLY);
  pidstring = iota(getpid(),pidstring,10);

  mkfifo(pidstring,0600);

  dtlength = strlen(pidstring)+1;
  write(w_k_p,&dtlength,sizeof(int));
  write(w_k_p,pidstring,dtlength);

  int secp = open(pidstring,O_RDONLY);
  
  return from_server;
}
