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
  char *dup_url = strdup(url);
  char *colon;
  char *slash;
  char *hostname;
  char *port;
  char *path;
  char *p;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  p = strstr(dup_url, "http://");
  if (p != NULL)
  {
    hostname = strdup(&p[7]);
  }
  else
  {
    p = strstr(dup_url, "https://");
    if (p != NULL)
    {
      hostname = strdup(&p[8]);
    }
    else {
      hostname = strdup(dup_url);
    }
  }

  colon = strchr(hostname, ':');
  port = colon + 1;
  *colon = '\0';
  slash = strchr(port, '/');
  path = slash + 1;
  *slash = '\0';

  urlinfo->hostname = strdup(hostname);
  urlinfo->port = strdup(port);
  urlinfo->path = strdup(path);

  free(dup_url);
  free(hostname);

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
