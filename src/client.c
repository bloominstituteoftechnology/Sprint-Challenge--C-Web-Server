#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

#define BUFSIZE 4096 // max number of bytes we can get at once

/**
 * Struct to hold all three pieces of a URL
 */
typedef struct urlinfo_t {
  char *hostname;
  char *port;
  char *path;
} urlinfo_t;

/**
 * Tokenize the given URL into hostname, path, and port.
 *
 * url: The input URL to parse.
 *
 * Store hostname, path, and port in a urlinfo_t struct and return the struct.
*/
urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *protocol= strdup(url);
  char *hostname;
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  /*
    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

char str1[15];
  strcpy(str1, "http");

  if (strncmp(str1, protocol, 4) == 0){ 
    
  char *tmp = strstr(protocol, "://");
  hostname = tmp + 3;
  *tmp = '\0';

  
  tmp = strstr(hostname, ":");
  port = tmp + 1;
  *tmp = '\0';
  tmp = strstr(port, "/");
  path = tmp + 1;
  *tmp = '\0';

  } else {

  hostname = protocol;
  char *tmp = strstr(protocol, ":");
  port = tmp + 1;
  *tmp = '\0';
  tmp = strstr(port, "/");
  path = tmp + 1;
  *tmp = '\0';
  }
 

  printf("%s\n", hostname);
  printf("%s\n", port);
  printf("%s\n", path);

//Initialize socket
int fd = get_socket(hostname, port);
  
if (fd < 0) {
      fprintf(stderr, "client: fatal error getting socket\n");
      exit(1);
}

// Sent request to server
send_request(fd, hostname, port, path);
  

return urlinfo;
}

/**
 * Constructs and sends an HTTP request
 *
 * fd:
 * hostname: The hostname string.
 * port:     The port string.
 * path:     The path string.
 *
 * Return the value from the send() function.
*/
int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  char *url;
  url = argv[1];
  parse_url(url);

  /*
    1. Parse the input URL
    2. Initialize a socket
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}
