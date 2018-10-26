#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

#define BUFSIZE 4096

typedef struct urlinfo_t {
  char *hostname;
  char *port;
  char *path;
} urlinfo_t;

urlinfo_t *parse_url(char *url)
{
  char *hostname = strdup(url);
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  char *tmp = strstr(hostname, ":");
  port = tmp + 1;
  urlinfo->port = port;
  *tmp = '\0';

  tmp = strstr(port, "/");
  path = tmp + 1;
  urlinfo->path = path;
  *tmp = '\0';

  urlinfo->hostname = hostname;

  return urlinfo;
}

int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;

  int request_length = sprintf(request,
                                "GET /%s HTTP/1.1\nHost: %s:%s\nConnection: close\n\n",
                                path, hostname, port);

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

  urlinfo_t *urlinfo = parse_url(argv[1]);
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0) {
    printf("%s", buf);
  }

  close(sockfd);
  free(urlinfo);

  return 0;
}
