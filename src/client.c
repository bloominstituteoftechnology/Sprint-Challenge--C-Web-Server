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
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(*urlinfo));
  port = strdup(strstr(hostname, ":") + 1);
  path = strdup(strstr(port, "/") + 1);

  hostname[strlen(hostname) - strlen(port) - 1] = '\0';
  port[strlen(port) - strlen(path) - 1] = '\0';

  urlinfo->hostname = hostname;
  urlinfo->port = port;
  urlinfo->path = path;

  return urlinfo;
}

int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;

  int request_length = sprintf(request, "GET /%s HTTP/1.1\n"
                                        "Host: %s:%s\n"
                                        "Connection: close\n"
                                        "\n",
                               path, hostname, port);

  rv = send(fd, request, request_length, 0);

  if (rv < 0)
  {
    perror("send");
  }
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
    1. Parse the input URL X
    2. Initialize a socket X
    3. Call send_request to construct the request and send it X
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  urlinfo_t *parsed_url = parse_url(argv[1]);
  sockfd = get_socket(parsed_url->hostname, parsed_url->port);
  send_request(sockfd, parsed_url->hostname, parsed_url->port, parsed_url->path);

  printf("*******reciver*******\n\n");

  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
  {
    // print the data we got back to stdout
    printf("%s\n", buf);
  }
  printf("*******reciver*******\n\n");

  return 0;
}
