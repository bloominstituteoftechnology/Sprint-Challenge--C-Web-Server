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
  char *hostname;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  char *http = strstr(url, "http://");
  char *https = strstr(url, "https://");

  if (http != NULL) {
    hostname = strdup(http + 7);
  } else if (https != NULL) {
    hostname = strdup(https + 8);
  } else {
    hostname = strdup(url);
  }

  char *path = strchr(hostname, '/');
  char *port = strchr(hostname, ':');

  if (path != NULL) {
    hostname[(int)(path - hostname)] = '\0';
    path = path + 1;
  } else {
    path = "/";
  }

  if (port != NULL) {
    hostname[(int)(port - hostname)] = '\0';
    port = port + 1;
  } else {
    port = "80";
  }

  urlinfo->hostname = hostname;
  urlinfo->path = path;
  urlinfo->port = port;

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

  int request_length = sprintf(request,
    "GET /%s HTTP/1.1\n"
    "Host: %s:%s\n"
    "Connection: close\n"
    "\n",
    path,
    hostname,
    port
  );

  int rv = send(fd, request, request_length, 0);

  return rv;
}

int main(int argc, char *argv[])
{
  int sockfd;
  char buf[65536];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  urlinfo_t *urlinfo = parse_url(argv[1]);

  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  if (sockfd == -1) {
    perror("Error getting socket");
  }

  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  while (recv(sockfd, buf, 65535, 0) > 0) {
    printf("%s\n", buf);
  }

  free(urlinfo->hostname);
  free(urlinfo);

  return 0;
}
