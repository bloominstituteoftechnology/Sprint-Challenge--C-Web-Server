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

urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *host;
  char *path;
  char http[10] = "http://";
  char https[10] = "https://";
  char www[10] = "www";

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  char *p;
 
  //if http or https exists in url
  if (strstr(url, http) != NULL || strstr(url, https) != NULL || strstr(url, www)) {

    //get the host first
    host = strtok(hostname, " ");
    puts(host);
    urlinfo->hostname = host;
    *urlinfo->port = 80;  //hardcoded for now
    *urlinfo->path = "/";


  } else {
    //get the hostname first
    host = strtok(hostname, ":");
    printf("this is hostname-%s\n", hostname);
//  then get the port
    port = strtok(NULL, "/");
    printf("this is port- %s\n", port);
    //then get the path
    path = strchr(url, '/');
    printf("this is path- %s\n", path);
    urlinfo->hostname = hostname;
    urlinfo->port = port;
    urlinfo->path = path;
  }



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

  if (rv < 0)
  {
    perror("send error");
  }

  return 0;
}

int main(int argc, char *argv[])
{  
  
  int numbytes;  
  char buf[BUFSIZE];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  } else {
    

    // 1. Parse the input URL
    urlinfo_t *urlinfo = parse_url(argv[1]);
    // 2. Initialize a socket
    int sockfd;
    sockfd = get_socket(urlinfo->hostname, urlinfo->port);

    // 3. Call send_request to construct the request and send it
    send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);
    // 4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.

    while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
    {
      //should print each line
      printf("%s\n", buf);
    }
    // 5. Clean up any allocated memory and open file descriptors.
    free(sockfd);
  }


    return 0;
}
