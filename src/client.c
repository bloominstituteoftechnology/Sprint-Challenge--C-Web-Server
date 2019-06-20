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

    1. Use strchr to find the first slash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the slash with a '\0' so that we are no longer considering anything after the slash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

  // Check the if the URL contains HTTP
  if (strstr(url, "http://")) 
  {
    // If URL contains 'http://'
    hostname = strdup(url + 7);

  } 
  else if (strstr(url, "https://")) 
  {
    // If URL contains 'https://'
    hostname = strdup(url + 8);

  } 
  else
   {
    // If URL does not contain any HTTP
    hostname = strdup(url);
  }

  // Set the path
  // If the hostname contains a /, set the path = /
  if (strchr(hostname, '/')) 
  {
    path = strchr(hostname, '/') + 1;

    *(path - 1) = NULL;
  }
   else 
  {
    path = '/';
  }

  // Set the port
  // If the hostname contains a :, set the port to the number after
  if (strchr(hostname, ':')) 
  {
    port = strchr(hostname, ':') + 1;

    *(port - 1) = NULL;
  } 
  else 
  {
    char buffer[3];

    sprintf(buffer, "%i", 80);

    port = buffer;
  }

  // Set the properties in the urlinfo

  urlinfo->hostname = hostname;

  urlinfo->path = strdup(path);

  urlinfo->port = strdup(port);

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

  // get request length
  int request_length = sprintf(
    request,
    "GET /%s HTTP/1.1\n"
    "HOST: %s:%s\n"
    "Connection: close\n"
    "\n",
    path,
    hostname,
    port
  );

  rv = send(fd, request, request_length, 0);

  return 0;
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];

  if (argc != 2)
  {
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

 // get struct values
  urlinfo_t *u = parse_url(argv[1]); 

  // get socket int val
  sockfd = get_socket(u->hostname, u->port); 

  // pass struct and socket values to send_request
  send_request(sockfd, u->hostname, u->port, u->path); 

  while((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0){
    printf("%s\n", buf);
  }

  free(u);
  close(sockfd);

  return 0;
}
