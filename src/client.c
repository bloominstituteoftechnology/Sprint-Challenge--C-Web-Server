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
  char *hostname = strdup(url);
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  /*
    The following is to isolate path and port strings and assign the values to their respective variables.
  */
  // returning pointer to index of first occurance of '/'
  path = strchr(hostname, '/');

  // assigning '\0' to index of path
  *path = '\0';
  // inrc the index of path pointer
  path++;

  // reassigning index of ':' to port
  port = strchr(hostname, ':');
  if( port == NULL ){
    // if nothing exists at port, port is assigned '80'
    port = '80';
  } else {
    // is value of port is not ':', '\0' is assigned to port index
    *port = '\0';
    // port index is incr.
    port++;
  }

  urlinfo->port = port;
  urlinfo->hostname = hostname;
  urlinfo->path = path;

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
  // Build HTTP response and store it in response
  int req_len = sprintf(request, 
  "GET /%s HTTP/1.1\n"
  "Host: %s:%s\n"
  "Connection: close",
  path, hostname, port
  );

  rv = send(fd, request, req_len, 0);

  if (rv < 0) {
      perror("send");
  }
  
  return 0;
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];
  char *URL = argv[1];
  printf("URL >>>> %s\n", URL);

  // 1. Parse the input URL
  struct urlinfo_t *urlinfo = parse_url(URL);

  // 2. Initialize socket
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);

  // 3. Call send_request to construct the request and send it
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  // 4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
  while((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0) {
    printf("Response:\n %s", buf);
    // fwrite()
  }

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  // 5. Clean up any allocated memory and open file descriptors.
  free(urlinfo);

  return 0;
}
