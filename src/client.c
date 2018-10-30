#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

#define BUFSIZE 4096 // max number of bytes we can get at once

char* split(char *s, const char *delim) {
  char *p = strstr(s, delim);
  
  if (p == NULL) return NULL;
  if (p == s + strlen(s) - 1) {
      *p = '\0';
      return NULL;
  }
  *p = '\0';
  
  return p + strlen(delim);
}

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
  char *uri, *tempuri1, *tempuri2, *tempuri3, *cpy_path;
  char *http = strdup(url);
  tempuri1 = split(http, "https://");

  if (tempuri1 == NULL) { 
    tempuri1 = strdup(url);
  }

  tempuri2 = split(tempuri1, "http://");

  if (tempuri2 == NULL) { 
    tempuri2 = strdup(tempuri1);
  }
  
  tempuri3 = split(tempuri2, "www.");

  if (tempuri3 == NULL) { 
    uri = strdup(tempuri2);
  } else {
    uri = strdup(tempuri3);
  }

  char *host, *port, *path;

  if (strchr(uri, ':')) {
    host = strdup(uri);
    port = split(host, ":");
    if (strchr(port, '/')) {
      path = split(port, "/");
      if(path == NULL) {
        path = strdup("/");
      } else {
        cpy_path = strdup(path);
        sprintf(path, "/%s", cpy_path);
      }
    } else {
      path = strdup("/");
    }
  } else {
    host = strdup(uri);
    port = strdup("80");
    if (strchr(host, '/')) {
      path = split(host, "/");
      if(path == NULL) {
        path = strdup("/");
      } else {
        cpy_path = strdup(path);
        sprintf(path, "/%s", cpy_path);
      }
    } else {
      path = strdup("/");
    }
  }

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));
  urlinfo->hostname = malloc(sizeof(host));
  urlinfo->port = malloc(sizeof(port));
  urlinfo->path = malloc(sizeof(path));

  strcpy(urlinfo->hostname, host);
  strcpy(urlinfo->port, port);
  strcpy(urlinfo->path, path);  

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

  sprintf(request, "GET %s HTTP/1.1\nHost: %s\nConnection:close\n\n", path,
                    hostname);
  puts(request);

  rv = send(fd, request, strlen(request), 0);
  if (rv < 0) return -1;
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

  /*
    1. Parse the input URL   
    2. Initialize a socket
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  char *url = strdup(argv[1]);

  urlinfo_t *url_info = parse_url(url);

  if (strcmp(url_info->hostname, "localhost") == 0) {
    sockfd = get_socket(url_info->hostname, url_info->port);
    int rv = send_request(sockfd, url_info->hostname, url_info->port, url_info->path);  
  
    if (rv == -1) {
        puts("Send failed");
        return -1;
    }
  } else {
    char new_hostname[100];
    sprintf(new_hostname, "www.%s", url_info->hostname);
    sockfd = get_socket(new_hostname, url_info->port);
    int rv = send_request(sockfd, new_hostname, url_info->port, url_info->path);  
  
    if (rv == -1) {
        puts("Send failed");
        return -1;
    }
  }

  char *leftover;

  if (strcmp(url_info->hostname, "localhost") == 0) {
    while ((numbytes = recv(sockfd, buf, BUFSIZ - 1, 0)) > 0) {
        printf("%.*s", numbytes, buf);
    }
  } else {
    while ((numbytes = recv(sockfd, buf, BUFSIZ - 1, 0)) > 0) {
        if (strstr(buf, "<!doctype")) {
            leftover = split(buf, "<!doctype");
            printf("%.*s", (int)strlen(buf), buf);
            break;
        } else if (strstr(buf, "<!DOCTYPE")) {
            leftover = split(buf, "<!DOCTYPE");
            printf("%.*s", (int)strlen(buf), buf);
            break;
        } else {
            printf("%.*s", numbytes, buf);
        }
    }
  }

  puts("\n"); 

  free(url_info->hostname);
  free(url_info->path);
  free(url_info->port);
  free(url_info);

  close(sockfd); 

  return 0;
}
