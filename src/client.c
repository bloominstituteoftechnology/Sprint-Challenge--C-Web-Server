#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"
 #include <arpa/inet.h>


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
  // printf("slash ptr: %s\n", temp_ptr);
  // 2. Set the path pointer to 1 character after the spot returned by strchr. 
  if (temp_ptr != NULL) {
    path = temp_ptr + 1;
    // 3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    *temp_ptr = '\0';
  }
  // printf("Path: %s\n", path);
  
  // printf("hostname: %s\n", hostname);

  // 4. Use strchr to find the first colon in the URL.
  temp_ptr = strchr(hostname, ':');

  // 5. Set the port pointer to 1 character after the spot returned by strchr.
  if (temp_ptr != NULL){
    port = temp_ptr + 1;
    // 6. Overwrite the colon with a '\0' so that we are just left with the hostname.
    *temp_ptr = '\0';
  }
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
  int request_length;
 
  // Build HTTP request and store it in request
  char full_path[50];
  strcpy(full_path,"/");
  strcat(full_path, path);
  char full_hostname[50];
  strcpy(full_hostname, hostname);
  strcat(full_hostname, ":");
  strcat(full_hostname, port);
  // printf("full path: %s, full hostname: %s\n", full_path, full_hostname);
  sprintf(request, "GET %s HTTP/1.1\nHost: %s\nConnection: close\n", full_path, full_hostname);

  request_length = strlen(request);
  // printf("Request: \n%s, request length: %d\n", request, request_length);
  int rv = send(fd, request, request_length+1, 0);
  if (rv < 0) {
      perror("send");
  }

  return rv;
  // return 0;
}

// void *get_in_addr(struct sockaddr *sa)
// {
//     if (sa->sa_family == AF_INET) {
//         return &(((struct sockaddr_in*)sa)->sin_addr);
//     }

//     return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }

int main(int argc, char *argv[])
{  
    int sockfd, numbytes;  
    char buf[BUFSIZE];

    if (argc != 2) {
      fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
      exit(1);
    }
    // printf("arg0: %s, arg1: %s\n", argv[0], argv[1]);
    // 1. Parse the input URL
    urlinfo_t *url_parsed;
    url_parsed = parse_url(argv[1]);
    // 2. Initialize a socket by calling the `get_socket` function from lib.c
    sockfd = get_socket(url_parsed->hostname, url_parsed->port);
    if (sockfd < 0) {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }
    printf("webserver: waiting for connections on port %s...\n", url_parsed->port);
    
    // printf("send socket: %d, hostname: %s, path: %s, port: %s\n", sockfd, url_parsed->hostname, url_parsed->path, url_parsed->port);
    // 3. Call `send_request` to construct the request and send it
    send_request(sockfd, url_parsed->hostname, url_parsed->port, url_parsed->path);
    
    // 4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    // int recvfd = get_socket(url_parsed->hostname, url_parsed->port);
    int buffer_length = strlen(buf);
    // struct sockaddr_storage their_addr; // connector's address information
    while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0) {
      fwrite(buf, 1, numbytes, stdout);
    }
    fclose(stdout);
    // while(1){
    //   printf("here");
      // socklen_t sin_size = sizeof their_addr;
      // recvfd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
      // if (recvfd == -1) {
      //     perror("accept");
      //     exit(1);
      // }
      // int rv = recv(recvfd, buffer, buffer_length, 0);
      // if (rv < 0) {
      //   perror("send");
      // }
      // inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
      // printf("client: got connection from %s\n", s);
      // printf("buffer: %s\n", buffer);
      // fprintf(stdout, "%s\n", buffer);
      // 5. Clean up any allocated memory and open file descriptors.
    //   close(sockfd);
      
    // }

  return 0;
}
