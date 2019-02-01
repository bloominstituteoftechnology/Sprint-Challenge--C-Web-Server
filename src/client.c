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
 
*/
urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));  

// 0. First you want to handle if the url has a https:// or http:// || could just add the amount of strings to the hostname
  if ( strstr(hostname, "https://") ) {
    hostname += 8;
    printf("%s", hostname);
  } else if ( strstr(hostname, "http://") ) {
    hostname += 7;
  } else {
    hostname = strdup(url);
  }

// 1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
// 2. Set the path pointer to 1 character after the spot returned by strchr. 
// 3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the forward-slash. 
  if ( strchr(hostname, "/") != NULL ) {      
    path = strchr(hostname, "/") + 1; // --> This is now POINTING to 1 char after the /   
    *(path - 1) = "\0";
    path++; // --> Return it back to what it was originally pointing at
  } else {
    fprintf(stderr, "Hostname + / == NULL\n");
    exit(1);
  }

// 4. Use strchr to find the first colon in the URL.
  if ( strchr(hostname, ":") != NULL ) {
// 5. Set the port pointer to 1 character after the spot returned by strchr.
    port = strchr(hostname, ":") + 1;
// 6. Overwrite the colon with a '\0' so that we are just left with the hostname.    
    *(port - 1) = "\0";
    port++; // --> Return it back to what it was originally pointing at
  } else {
    fprintf(stderr, "HOSTNAME + : == NULL\n");
    exit(1);
  }

// * Store hostname, path, and port in a urlinfo_t struct and return the struct.
  urlinfo->hostname = hostname;
  urlinfo->path = path;
  urlinfo->port = port;

    

  return urlinfo;
}

/**
 * Constructs and sends an HTTP request
 *
 * fd:       The file descriptor of the connection.
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

  /*
  GET /path HTTP/1.1
  Host: hostname:port
  Connection: close
  */

  int request_length = sprintf(request,
    "GET /%s HTTP/1.1\n"
    "Host: %s:%s\n"
    "Connection: close",
    path,
    hostname,
    port
  );

// Send it all
  rv = send(fd, request, request_length, 0);
  
  if (rv < 0) {
    perror("send");
  }

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

  /*
    1. Parse the input URL
    2. Initialize a socket by calling the `get_socket` function from lib.c
    3. Call `send_request` to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}
