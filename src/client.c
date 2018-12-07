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

urlinfo_t *parse_url(char *url)
{
  char *hostname = strdup(url);
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

    char portStr = strchr(url, '/');
    *port = '\0';
    char *portMem = strdup(portStr);
    port++;

    char pathStr = strchr(url, ':');
    *path = "\0";
    char *pathMem = strdup(pathStr);
    path++;

printf("Hostname: %s\n", hostname);
printf("Port: %s\n", port);
printf("Path: %s\n", path);

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
  int request_length = sprintf(request, "File Descriptor: %d\nHostname: %s\nPort: %s\nPath:%s\n", fd, hostname, port, path);

  rv = send(fd, request, request_length, 0);
  if(rv < 0){
    perror("send");
  }

  return 0;
}



int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }


  struct urlinfo_t *parseURL = malloc(sizeof *parseURL);
  
  // parseURL = parse_url(argv);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1){
    printf("socket error\n");
    exit(1);
  }

  int sendRequest = send_request(sockfd, parseURL->hostname, parseURL->port, parseURL->path);

  numbytes = recv(sockfd, buf, BUFSIZ - 1, 0);


  if(numbytes < 0 ){
    perror("recv");
    return;
  }

  char hostname[256];
  char port[256];
  char path[256];

  while(numbytes > 0){
    fprintf(stdout, "%s\n", hostname);
    fprintf(stdout, "%s\n", port);
    fprintf(stdout, "%s\n", path);

  }

  free(parseURL);

  return 0;
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

// int send_request(int fd, char *hostname, char *port, char *path)







// Old Parse_url function
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include "lib.h"

// #define BUFSIZE 4096 // max number of bytes we can get at once

// /**
//  * Struct to hold all three pieces of a URL
//  */
// typedef struct urlinfo_t {
//   char *hostname;
//   char *port;
//   char *path;
// } urlinfo_t;

// /**
//  * Tokenize the given URL into hostname, path, and port.
//  *
//  * url: The input URL to parse.
//  *
//  * Store hostname, path, and port in a urlinfo_t struct and return the struct.
// */
// urlinfo_t *parse_url(char *url)
// {
//   // copy the input URL so as not to mutate the original
//   char *hostname = strdup(url);
//   char *port;
//   char *path;

//   urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  
//     // We can parse the input URL by doing the following:

//     // 1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
//     char backslash = strchr(url, "/");
//     while(backslash){
//       urlinfo->path = 1;
//       backslash = "\0";
//     };

//     // 2. Set the path pointer to 1 character after the spot returned by strchr.

//     // 3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    
//     // 4. Use strchr to find the first colon in the URL.
//     path = strchr(url, ":");
//     *path = "\0";
//     path++;
//     // while (colon) {
//     //   urlinfo->port = 1;
//     //   colon = "\0";
//     // }
//     // 5. Set the port pointer to 1 character after the spot returned by strchr.
    
//     // 6. Overwrite the colon with a '\0' so that we are just left with the hostname.
//   return urlinfo;
// }
