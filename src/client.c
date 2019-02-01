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

  char backslash = '/';
  char *backslash_ptr;
  char colon = ':';
  char *colon_ptr;

  // 1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
  backslash_ptr = strchr(hostname, backslash);
  // 2. Set the path pointer to 1 character after the spot returned by strchr.
  path = backslash_ptr + 1;
  // 3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
  *backslash_ptr = '\0';
  // 4. Use strchr to find the first colon in the URL.
  colon_ptr = strchr(hostname, colon);
  // 5. Set the port pointer to 1 character after the spot returned by strchr.
  port = colon_ptr + 1;
  // 6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  *colon_ptr = '\0';

  urlinfo->hostname = hostname;
  urlinfo->port = port;
  urlinfo->path = path;

  printf("hostname, port, path: %s, %s, %s\n", hostname, port, path);
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

  printf("\n");
  char *url = "localhost:3490/d20";
  parse_url(url);
  printf("\n");

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
