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
  char *port = strchr(hostname, ':'); //Use strchr to find the first colon in the URL.
  char *path = strchr(hostname, '/'); //Use srtchr to find the first backslash in the URL.

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  if (path != NULL)
  {
    //Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    hostname[(int)(path - hostname)] = '\0';
    //Set the path pointer to 1 character after the spot returned by strchr.
    path = path + 1;
  }
  else
  {
    path = "/";
  }

  if (port != NULL)
  {
    //Overwrite the colon with a '\0' so that we are just left with the hostname.
    hostname[(int)(port - hostname)] = '\0';
    //Set the port pointer to 1 character after the spot returned by strchr.
    port = port + 1;
  }
  else
  {
    //default port number used (80) for Stretch Goal 1.
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

  //Construct HTTP request here
  int request_length = sprintf(request,
      "\n"
      "GET /%s HTTP/1.1\n"
      "Host:%s:%s\n" //hostname:port
      "Connection: close\n\n",
      hostname,
      port,
      path
  );
  //moved int rv here to be able to add request_length as param
  int rv = send(fd, request, request_length, 0);
  //return value
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
  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  // 1. Parse the input URL
  urlinfo_t *urlinfo = parse_url(argv[1]);

  //2. Initialize a socket (if no socket get error)
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  if (sockfd == -1)
  {
    perror("Unable to retrieve the socket");
  }

  //3. Call send_request to construct the request and send it
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  //4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
  while ((numbytes = recv(sockfd, buf, 65535, 0)) > 0)
  {
    fprintf(stdout,"%s", buf);
  }

  //5. Clean up any allocated memory and open file descriptors.

  free(urlinfo->hostname);
  free(urlinfo);
  close(sockfd);

  return 0;
}
