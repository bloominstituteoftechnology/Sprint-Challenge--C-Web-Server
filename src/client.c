#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

#define BUFSIZE 4096 // max number of bytes we can get at once

typedef struct urlinfo_t {
  char *hostname;
  char *port;
  char *path;
} urlinfo_t;

char *remove_char(char *str, char c)
{
  for (int i = 0; i <= strlen(str); i++)
  {
    if (str[i] == c)
    {
      str[i] = '\0';
    }
  }
  return str;
}

urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *path;
  char *curr;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  curr = strchr(hostname, '/');
  path = curr + 1;
  hostname = remove_char(hostname, '/');

  curr = strchr(hostname, ':');
  port = curr + 1;
  hostname = remove_char(hostname, ':');

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

  int request_length = sprintf(request,
    "GET /%s HTTP/1.1\n"
    "Host: %s:%s\n"
    "Connection: close\n"
    "\n",
    path, hostname, port);

  rv = send(fd, request, request_length, 0);

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
  urlinfo_t *url = parse_url(argv[1]);
  sockfd = get_socket(url->hostname, url->port);
  send_request(sockfd, url->hostname, url->port, url->path);

  while((numbytes = recv(sockfd, buf, BUFSIZ -1, 0)) > 0)
  {
    printf("%s\n", buf);
  }
  close(sockfd);
  free(url);

  return 0;
}
