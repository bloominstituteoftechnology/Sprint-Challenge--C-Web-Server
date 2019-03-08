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
 *
 * Store hostname, path, and port in a urlinfo_t struct and return the struct.
*/
urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *path;
  char *slash;
  char *colon;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  // Find first backslash in URL
  // TODO: Check for protocol in URL (ie. http://, https://)
  slash = strchr(hostname, '/');
  // set path pointer to 1 character after slash
  path = slash + 1;
  // Set backslash to '\0'
  *slash = '\0';

  // Find first colon in URL
  colon = strchr(hostname, ':');
  // set port to 1 character after the colon
  port = colon + 1;
  // Set colon to '\0'
  *colon = '\0';

  // Assign to fields on urlinfo
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
  // Build HTTP request
  int request_length = sprintf(request,
                               "GET /%s HTTP/1.1\n"
                               "Host: %s:%s\n"
                               "Connection: close\n"
                               "\n",
                               path,
                               hostname,
                               port);

  // Send it all!
  rv = send(fd, request, request_length, 0);

  if (rv < 0)
  {
    perror("send");
  }

  return rv;
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

  /*
    1. Parse the input URL
    2. Initialize a socket by calling the `get_socket` function from lib.c
    3. Call `send_request` to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  // Parse input URL
  urlinfo_t *urlinfo = parse_url(argv[1]);
  // Initialize socket
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  // call send_request to contruct the request and send it
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);
  // Call 'recv' in a loop until there is no more data to receive from the server
  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
  {
    // Print out response to stdout
    fprintf(stdout, "%s\n", buf);
  }

  // Clean allocated memory
  free(urlinfo->hostname);
  free(urlinfo);

  // Close file descriptor
  close(sockfd);

  return 0;
}
