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

  char *httpsStart = strstr(url, "https://");
  if (httpsStart != NULL) {
    url += 8;
  }

  char *httpStart = (strstr(url, "http://"));
  if (httpStart != NULL) {
    url += 7;
  }

  char *pathStart = strstr(url, "/");
  if (pathStart != NULL) {
    urlinfo->path = pathStart + 1;
    *pathStart = '\0';
  } else {
    urlinfo->path = "";
  }

  char *portStart = strstr(url, ":");
  if (portStart != NULL) {
    urlinfo->port = portStart +1;
    *portStart = '\0';
  } else {
    urlinfo->port = "80";
  }

  urlinfo->hostname = url;

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

  rv = sprintf(request, "GET /%s HTTP/1.1\nHost: %s:%s\nConnection: close\n\n", path, hostname, port);
  // printf("%s", request);

  rv = send(fd, request, rv, 0);
  if (rv < 0) {
    perror("send");
  }

  return rv;
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

  struct urlinfo_t *urlinfo = parse_url(argv[1]);
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  while ((numbytes = recv(sockfd, buf, BUFSIZE-1, 0)) > 0) {
    fprintf(stdout, "%s", buf);
    fflush(stdout);
  }
  printf("\n");

  close(sockfd);
  if (urlinfo != NULL) {
    free(urlinfo);
  }

  return 0;
}
