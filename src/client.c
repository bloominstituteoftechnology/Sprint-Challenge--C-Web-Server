#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
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

  // Stretch added by solution(remove http://):

  char *tmp = strstr(hostname, "://");
  if (tmp != NULL) {
    hostname = tmp + 3;
  }
  // Step 1 - Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
  // Step 2 - Set the path pointer to 1 character after the spot returned by strchr.
  // Step 3 - Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
  tmp = strstr(hostname, "/");
  path = tmp + 1;
  *tmp = '\0';
  // Step 4 - Use strchr to find the first colon in the URL.
  // Step 5 - Set the port pointer to 1 character after the spot returned by strchr.
  // Step 6 - Overwrite the colon with a '\0' so that we are just left with the hostname.
  tmp = strstr(hostname, ":");
  if (tmp == NULL) {
    port = "80";
  } else {
    port = tmp + 1;
    *tmp = '\0';
  }

  urlinfo->hostname = hostname;
  urlinfo->port = port;
  urlinfo->path = path;

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

  time_t t = time(NULL);
  struct tm *lt = localtime(&t);

  unsigned long int request_length = sprintf(request,
    "GET /%s HTTP/1.1\nHost: %s:%s\nConnection: close\n\n",
    path, hostname, port);

  rv = send(fd, request, request_length, 0);

  return rv;
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];
  char hostname[18], port[8], path[20];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  // Step 1 - Parse the input URL
  struct urlinfo_t *urlinfo = parse_url(argv[1]);

  // Step 2 - Initialize a socket
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);

  // Step 3 - Call send_request to construct the request and send it
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  // Step 4 - Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
  while ((numbytes = recv(sockfd, buf, BUFSIZE -1, 0)) > 0) {
    // fprintf(stdout, "%s\n", buf);
    fwrite(buf, 1, numbytes, stdout);
  }

  // Step 5 - Clean up any allocated memory and open file descriptors.
  free(urlinfo);
  close(sockfd);

  return 0;
}
