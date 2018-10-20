#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

// header: HTTP/1.1 200 OK || HTTP/1.1 200 OK
// body: the data to send.
// Date: Wed Dec 20 13:05:11 PST 2017
// v     v       v        v
//     time_t time1 = time(NULL);
//     struct tm *localtime1 = localtime(&time1);
//     asctime(localtime1)


// Connection: close
// Content-Length: 41749
// Content-Type: text/html

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

  char *backslash;
  char *colon;

  backslash = strchr(url, '/');

  backslash++;
  path = backslash;

  // printf("PATH -> %s\n", backslash);

  *backslash = '\0';

  colon = strchr(url, ':');

  colon++;
  port = colon;

// printf("PORT -> %s\n", colon);

 *colon = '\0';

//  printf("%s\n", url);

hostname = url;

urlinfo->path = path;
urlinfo->port = port;
urlinfo->hostname = hostname;

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
  int rv;

  time_t time1 = time(NULL);
  struct tm *localtime1 = localtime(&time1);


  
  // stringify the passed data into the request packet buffer
  int request_length = sprintf(response,
      "%s\n"
      "Date: %s\n" // asctime
      "Port: %s\n"
      "Path: %s\n"
      "Content-Length: %d\n"
      "\n" // end

      hostname,
      asctime(localtime1),
      port,
      path,
      request_length
      );


  // send function
  int rv = send(fd, request, request_length, 0);


  // error checking, if value returned is -1
  if (rv < 0) {
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

  /*
    1. Parse the input URL
    2. Initialize a socket
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  parse_url(argv);
  get_socket(*hostname, *port);
  send_request(sockfd, *hostname, *port, *path);


  // might be wrong
  int num_recv = recv(sockfd, buf, BUFSIZE - 1, 0);

  while (num_recv < 0) {
    printf();
  }

  free();

  return 0;
}
