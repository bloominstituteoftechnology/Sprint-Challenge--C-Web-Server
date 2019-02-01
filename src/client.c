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

  /*
    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

  path = strchr(url, '/');
  // printf("path: %s\n", path);

  for(int i = 0; i < strlen(url); i++){
    if(url[i] == '/'){
      url[i] = '\0';
    }
  }

  // printf("null slash: %s\n", url);

  path = &path[1];
  urlinfo->path = path;
  // printf("path no slash: %s\n", path);
  // printf("struct path: %s\n", urlinfo->path);

  port = strchr(url, ':');
  // printf("port: %s\n", port);
  port = &port[1];
  // printf("port no colon: %s\n", port);

  urlinfo->port = port;

  for(int i = 0; i < strlen(url); i++){
    if(url[i] == ':'){
      url[i] = '\0';
    }
  }

  // printf("hostname: %s\n", url);

  urlinfo->hostname = url;

  // printf("%s, %s, %s\n", urlinfo->hostname, urlinfo->port, urlinfo->path);


  ///////////////////
  //   COMPLETE!   //
  ///////////////////

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

  // EXAMPLE REQUEST //
  // GET /path HTTP/1.1
  // Host: hostname:port
  // Connection: close

  int request_length = sprintf(request, "GET /%s HTTP/1.1\nHost: %s:%s\nConnection: close\n", path, hostname, port);

  // printf("request:\n%s\n", request);
  // send will return -1 on error, number of chars send if successful
  rv = send(fd, request, request_length, 0);

  if(rv < 0){
    fprintf(stderr, "No data received from request.\n");
    exit(1);
  }

  // printf("request_length: %d\n", request_length);
  // printf("rv: %d\n", rv);

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

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

  // printf("%s\n", argv[1]);
  urlinfo_t *urlinfo = parse_url(argv[1]);

  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  // printf("sockfd: %d\n", sockfd);

  numbytes = send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  while((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0){
    // print the data to stdout
    // printf("Receiving...\n");
    printf("%s\n", buf);
  }

  close(sockfd);
  
  urlinfo->port = NULL;
  free(urlinfo->port);
  urlinfo->hostname = NULL;
  free(urlinfo->hostname);
  urlinfo->path =NULL;
  free(urlinfo->path);
  free(urlinfo);

  // printf("numbytes: %d\n", numbytes);


  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}
