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
    localhost:3490/d20
    ^         ^    ^ 
    hostname  port path

    The hostname is everything before the colon but doesn't include http:// or https://
    The port is the number after the colon ending at the backslash
    The path is everthing after the backslash

    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  //parsing the hostname:
  //use strchr to find the first backslash in the URL
  char *temp = strchr(hostname,'/'); // returns a pointer to the '/'
  //set the path pointer to 1 character after the spot returned by strchr
  path = temp + 1;
  //overwrite the backslash with a '\0'
  *temp = '\0';

  //parsing the path
  //use strchr to find the first ':' in the url
  temp = strchr(hostname, ':'); //returns a pointer to the colon
  //set the port pointer to 1 character after the spot returned by the strchr
  port = temp + 1;
  //overwrite the colon with a '\0' so that we are just left with the hostname
  *temp = '/0';

  //Assign each of these to the appropriate field in the urlinfo_t struct
  urlinfo->hostname = strdup(hostname);
  urlinfo->port = strdup(port);
  urlinfo->path = strdup(path);

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
 * Requests should look like the following:

  GET /path HTTP/1.1
  Host: hostname:port
  Connection: close
 
 * Return the value from the send() function.
*/
int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int request_length = 0; //following pattern in server.c, this gives total length of header and body
  int rv;

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  request_length = sprintf(
    request,
    //set up formatting for subsequent arguments
    "GET /%s\n Host: %s:%s\nConnection: close\n",
    
    path, hostname, port
  );


    rv = send(fd, request, request_length, 0);

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
    2. Initialize a socket
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}
