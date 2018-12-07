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

  printf("\n--in parse_url--\n\n");
  
  // We can parse the input URL by doing the following:

  // 1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
  char *clean;
  clean = strstr(url, "://");
  printf("clean: %s\n", clean);
  if(clean != NULL){
    url = clean +3;
  }
  printf("url: %s\n", url);
  
  path = strchr(url, '/');
  printf("path: %s\n", path);

  // 2. Set the path pointer to 1 character after the spot returned by strchr.

  urlinfo->path = path +1;
  printf("urlinfo->path: %s\n", urlinfo->path);

  // 3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.

  *path = '\0';
  printf("url: %s\n", url);

  // 4. Use strchr to find the first colon in the URL.

  port = strchr(url, ':');
  printf("port: %s\n", port);

  // 5. Set the port pointer to 1 character after the spot returned by strchr.
  
  if(port == NULL){
    urlinfo->port = "80";
  } else {
    urlinfo->port = port +1;
  // 6. Overwrite the colon with a '\0' so that we are just left with the hostname.
    *port = '\0';
  }
  printf("urlinfo->port: %s\n", urlinfo->port);
  printf("url: %s\n", url);

  urlinfo->hostname = url;
  printf("urlinfo->hostname: %s\n", urlinfo->hostname);

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

  printf("\n--inside send_request--\n\n");
  printf("fd: %d\n", fd);

  int *request_length = sprintf(request, "GET /%s HTTP/1.1\nHost: %s:%s\nConnection: close\n\n", path, hostname, port);//may or many not need the extra new line at the end 
  printf("request_length: %i\n", request_length);
  printf("request:\n'%s'\n", request);

  rv = send(fd, request, request_length, 0);

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return rv;//changed from 0 to rv
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  //1. Parse the input URL
  char *hostname = strdup(argv[1]);
  printf("%s\n", hostname);
  struct urlinfo_t *parsedUrl = parse_url(hostname);
  printf("\n--after parse_url--\n\n");
  printf("hostname: %s\n", parsedUrl->hostname);
  printf("path: %s\n", parsedUrl->path);
  printf("port: %s\n", parsedUrl->port);
  //2. Initialize a socket
  sockfd = get_socket(parsedUrl->hostname, parsedUrl->port);
  // printf("sockfd: %i\n", sockfd);

  //3. Call send_request to construct the request and send it
  numbytes = send_request(sockfd, parsedUrl->hostname, parsedUrl->port, parsedUrl->path);
  // printf("numbytes: %i\n", numbytes);
  // printf("BUFSIZE: %i\n", BUFSIZE);

  //4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.

  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0) {
       // print the data we got back to stdout
       buf[numbytes] = '\0';
       printf("%s\n", buf);
       //beej----- fwrite(buf, sizeof(char), numbytes, stdout)

       //vause that makes it null terminate 

        char *firstLine;
        char *secondLine;
        char *everythingElse;
        firstLine = buf;
        secondLine = strchr(firstLine, '\n');
        *secondLine = '\0';
        secondLine++;
        printf("firstLine: %s\n", firstLine);
        everythingElse = strchr(secondLine, '\n');
        *everythingElse = '\0';
        everythingElse++;
        printf("secondLine: %s\n", secondLine);
        // printf("everythingElse: %s\n", everythingElse);
        char *is301;
        char *redirectURL;
        is301 = strstr(firstLine, "301");
        printf("is301 %s", is301);
        if(is301 != NULL){
          printf("301 is not null");
          redirectURL = strstr(secondLine, "http");
          printf("%s", redirectURL);
          char *newArgv;
          sprintf(newArgv, "./client %s", redirectURL);
          main(2, newArgv);
        } else {
          break;
        }
     }


  //5. Clean up any allocated memory and open file descriptors.

  free(parsedUrl);

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}
