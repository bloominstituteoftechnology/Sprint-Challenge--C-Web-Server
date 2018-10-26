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
  

  char *tmp = strstr(hostname, "/");
  path = tmp + 1;
  *tmp = '\0';

  tmp = strstr(path, ":");
  port = tmp + 1;
  *tmp = '\0';

  urlinfo->hostname = hostname;
  urlinfo->port = port;
  urlinfo->path = path;

  printf("%s\n", hostname);
  printf("%s\n", path);
  printf("%s\n", port);


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
// unsigned long int response_length = sprintf(request, "%s\n%s\n%s\n", something, something, something);

// GET /path HTTP/1.1
// Host: hostname:port
// Connection: close

int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;
  
  char get[30];
  char connection[30];
  char host[30];

    // sprintf(hostname, "Hostname: %lu", hostname);
    // sprintf(port, "Port: %s", port);
    // sprintf(path, "Path: ",path);
    sprintf(get, "GET /%s HTTP/1.1", path);
    sprintf(host, "Host: %s:%s", hostname, port);
    sprintf(connection, "Connection: close");

    unsigned long int request_length = sprintf(request, "%s\n%s\n%s\n", get, host, connection);

    // rv = 
    send(fd, request, request_length, 0 );
    // return rv;

    
    
  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  

  return 0;
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];

  if (argc != 2) {
    printf("%s\n",argv);
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  /*
    1. Parse the input URL
    2. Initialize a socket
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */
  struct urlinfo_t *url = parse_url(argv[1]);
  printf("%s",argv);
  // get_in_addr(argv);
  sockfd = get_socket(url->hostname, url->port);
  printf("%s",sockfd);
  // send_request( fd,argv[0], argv[1], argv[2]);
  send_request(sockfd, url->hostname, url->port, url->path);
  while((numbytes = recv(sockfd, buf, BUFSIZE-1, 0)) > 0){
    fprintf 
  };
  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}
