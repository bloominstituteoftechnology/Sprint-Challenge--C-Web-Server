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
    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
  */
  path = strchr(hostname, '/');
  *path = '\0';
  path += 1;

/*
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
*/
  port = strchr(hostname, ':');
  *port = '\0';
  port += 1;

// Next three lines of code storing strings in urlinfo structure
  urlinfo -> hostname = hostname;
  urlinfo -> port = port;
  urlinfo -> path = path;

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

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  int request_length = sprintf(
        request,
        "GET /%s HTTP/1.1\nHOST: %s:%s\nConnection: Close\n\n",
        path, hostname, port
    );

// Send it all........
  rv = send(fd, request, request_length, 0);

  if (rv < 0) 
  {
    perror("Send");
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

  
    // 1. Parse the input URL
    urlinfo_t *urlinfo = parse_url(argv[1]);

    // 2. Initialize a socket
    sockfd = get_socket(urlinfo -> hostname, urlinfo -> port);

    // 3. Call send_request to construct the request and send it
    send_request(sockfd, urlinfo -> hostname, urlinfo -> port, urlinfo -> path);
    // 4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
    {
      // print the data we got back to stdout
      printf("%s", buf);
    }
    // 5. Clean up any allocated memory and open file descriptors.
    free(urlinfo);
    close(sockfd);

    return 0;
}
