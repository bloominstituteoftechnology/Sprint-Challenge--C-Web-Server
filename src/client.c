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
  char *https = strstr(hostname, "https://");
  if (https != NULL)
  {
    hostname = https + strlen("https://");
    *https = '\0';
  }
  else
  {
    char *http = strstr(hostname, "http://");

    if (http != NULL)
    {
      hostname = http + strlen("http://");
      *http = '\0';
    }
  }

  char *first_backslash = strchr(hostname, '/');
  path = first_backslash + 1;
  *first_backslash = '\0';
  char *first_colon = strchr(hostname, ':');
  if (first_colon == NULL)
  {
    port = "80";
    urlinfo->port = port;
  }
  else
  {
    port = first_colon + 1;
    *first_colon = '\0';
    urlinfo->port = port;
  }
  
  urlinfo->path = path;
  urlinfo->hostname = hostname;
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

  sprintf(request, "GET /%s HTTP/1.1\n"
    "Host: %s:%s\n"
    "Connection: close\n\n", path, hostname, port);
  printf("%s\n", request);
  rv = send(fd, request, strlen(request), 0);

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

  urlinfo_t *urlinfo = parse_url(argv[1]);
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  int rv = send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  if (rv < 0) {
      perror("send");
  }

  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
  {
    printf("%s\n", buf);
  }
  free(urlinfo->hostname);
  free(urlinfo);
  close(sockfd);

  return 0;
}
