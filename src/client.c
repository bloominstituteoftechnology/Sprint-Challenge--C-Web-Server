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
  char *port = "";
  char *path = "";

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));
  // 1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
  char *temp_ptr;
  temp_ptr = strchr(hostname, '/');
  printf("slash ptr: %s\n", temp_ptr);
  // 2. Set the path pointer to 1 character after the spot returned by strchr. 
  if (temp_ptr != NULL) {
    path = temp_ptr + 1;
    // 3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    *temp_ptr = '\0';
  }
  printf("Path: %s\n", path);
  
  // printf("hostname: %s\n", hostname);

  // 4. Use strchr to find the first colon in the URL.
  temp_ptr = strchr(hostname, ':');

  // 5. Set the port pointer to 1 character after the spot returned by strchr.
  if (temp_ptr != NULL){
    port = temp_ptr + 1;
    // 6. Overwrite the colon with a '\0' so that we are just left with the hostname.
    *temp_ptr = '\0';
  }
  
  printf("hostname: %s, path: %s, port: %s\n", hostname, path, port);

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
// int send_request(int fd, char *hostname, char *port, char *path)
// {
//   const int max_request_size = 16384;
//   char request[max_request_size];
//   int request_length;

//   // Initiate variables for displaying time
//   time_t rawtime;
//   struct tm * timeinfo;
//   time (&rawtime);
//   timeinfo = localtime (&rawtime); 
//   extern char *tzname[2];

//   //build header: first line, 

//   // Build HTTP request and store it in request
//   sprintf(request, "%s\nDate: %sConnection: close\nContent-Length: %d\nContent-Type: %s\n\n%s", header, asctime(timeinfo), content_length, content_type, body);

//   request_length = strlen(request);

//   int rv = send(fd, request, request_length+1, 0);
//   if (rv < 0) {
//       // printf("rv <0\n");
//       perror("send");
//   }

//   return rv;
// }

int main(int argc, char *argv[])
{  
    int sockfd, numbytes;  
    char buf[BUFSIZE];

    if (argc != 2) {
      fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
      exit(1);
    }

    // 1. Parse the input URL
    urlinfo_t *url_parsed = parse_url(argv[1]);
    // 2. Initialize a socket by calling the `get_socket` function from lib.c
    int new_socket = get_socket(url_parsed->hostname, url_parsed->port);
    
    // 3. Call `send_request` to construct the request and send it
    // send_request(new_socket, url_parsed->hostname, url_parsed->port, url_parsed->path);
    
    // 4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    while(1){

    }
    // 5. Clean up any allocated memory and open file descriptors.

  return 0;
}
