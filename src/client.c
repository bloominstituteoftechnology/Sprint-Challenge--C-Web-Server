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
typedef struct urlinfo_t
{
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
  char *port = port;
  char *path = path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  char *http = strstr(hostname, "://");

  if (http != NULL)
  {
    hostname = http + 3;
  }

  http = strchr(hostname, '/');
  path = http + 1;
  *http = '\0';

  http = strchr(hostname, ':');
  port = http + 1;
  *http = '\0';

  urlinfo->hostname = strdup(hostname);
  urlinfo->port = strdup(port);
  urlinfo->path = strdup(path);

  return urlinfo;
}

void free_urlinfo(struct urlinfo_t *urlinfo)
{
  free(urlinfo->hostname);
  free(urlinfo->port);
  free(urlinfo->path);
  free(urlinfo);
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

  int request_length = sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s:%s\r\nConnection: close\r\n\r\n", path, hostname, port);

  int rv = send(fd, request, request_length, 0);

  return rv;
}

int main(int argc, char *argv[])
{
  int sockfd, numbytes;
  char buf[BUFSIZE];

  if (argc != 2)
  {
    fprintf(stderr, "usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  /*
    1. Parse the input URL
    2. Initialize a socket
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  struct urlinfo_t *urlinfo = parse_url(argv[1]);

  char *hostname = urlinfo->hostname;
  char *port = urlinfo->port;
  char *path = urlinfo->path;

  sockfd = get_socket(hostname, port);

  send_request(sockfd, hostname, port, path);

  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
  {
    printf("%s\n", buf);
  }

  return 0;
}
