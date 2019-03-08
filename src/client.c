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
 
 
*/
urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  // 0. STRETCH: First you want to handle if the url has a https:// or http:// || could just add the amount of strings to the hostname
  if (strstr(hostname, "https://"))
  {
    printf("We found an HTTPS\n");
    hostname += 8;
    printf("%s\n", hostname);
  }
  else if (strstr(hostname, "http://"))
  {
    printf("We found an HTTP\n");
    hostname += 7;
    printf("%s\n", hostname);
  }
  else
  {
    printf("We found a REGULAR URL\n");
    hostname = strdup(url);
  }

  char *backslash = strchr(hostname, '/');
  path = backslash + 1;
  *backslash = '\0';

  char *colon = strchr(hostname, ':');
  if (colon)
  {
    port = colon + 1;
    *colon = '\0';
  }
  else
  {
    port = "80";
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
                               "Connection: close\n\n",
                               path,
                               hostname,
                               port);

  // Send it all
  rv = send(fd, request, request_length, 0);

  if (rv < 0)
  {
    perror("send");
  }

  return 0;
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

  urlinfo_t *urlinfo = parse_url(argv[1]);
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
  {
    printf("%s\n", buf);
  }

  free(urlinfo);
  close(sockfd);

  return 0;
}
