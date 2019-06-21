#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

#define BUFSIZE 4096 // max number of bytes we can get at once
#define MAX_PROTOCOL_LEN 16

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

  char *httpsStart = strstr(url, "https://");
  if (httpsStart != NULL) {
    url += 8;
  }

  char *httpStart = (strstr(url, "http://"));
  if (httpStart != NULL) {
    url += 7;
  }

  char *pathStart = strstr(url, "/");
  if (pathStart != NULL) {
    urlinfo->path = pathStart + 1;
    *pathStart = '\0';
  } else {
    urlinfo->path = "";
  }

  char *portStart = strstr(url, ":");
  if (portStart != NULL) {
    urlinfo->port = portStart +1;
    *portStart = '\0';
  } else {
    urlinfo->port = "80";
  }

  urlinfo->hostname = url;

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

  rv = sprintf(request, "GET /%s HTTP/1.1\nHost: %s:%s\nConnection: close\n\n", path, hostname, port);
  // printf("%s", request);

  rv = send(fd, request, rv, 0);
  if (rv < 0) {
    perror("send");
  }

  return rv;
}

int handle_redirect(int fd, struct urlinfo_t *urlinfo, char *response) {
  int rv = 0;
  char *redirect = NULL;
  // Find the start of the url
  if ((redirect = strstr(response, "Location: ")) != NULL || (redirect = strstr(response, "location: ")) != NULL) {
    redirect += 10;
    // Trim off the rest of the response
    char *newline = strstr(redirect, "\r");
    if (newline != NULL) {
      *newline = '\0';
    } else if ((newline = strstr(redirect, "\n")) != NULL) {
      *newline = '\0';
    }
    printf("Got a 301 response. Redirecting to: %s\n", redirect);
    // Reset variables and make a new request.
    urlinfo = parse_url(redirect);
    close(fd);
    fd = get_socket(urlinfo->hostname, urlinfo->port);
    rv = send_request(fd, urlinfo->hostname, urlinfo->port, urlinfo->path);
  }
  return rv;
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];

  if (argc < 2 || argc > 3) {
    fprintf(stderr,"usage: client [-h] HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  int hide_header = strcmp(argv[1], "-h");
  char *url = argv[argc - 1];

  struct urlinfo_t *urlinfo = parse_url(url);
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  int is_first_pass = 1;
  while ((numbytes = recv(sockfd, buf, BUFSIZE-1, 0)) > 0) {
    if (is_first_pass) {
      // Check for redirect
      char *redirect = strstr(buf, "301");
      int handled_redirect = 0;
      if (redirect != NULL && (redirect - buf) < MAX_PROTOCOL_LEN) {
        handled_redirect = handle_redirect(sockfd, urlinfo, redirect);
        continue;
      } else if (hide_header && (redirect = strstr(buf, "\r\n\r\n")) != NULL) {
        // Skip header
        redirect += 4;
        fprintf(stdout, "%s", redirect);
        fflush(stdout);
      } else if (hide_header && ((redirect = strstr(buf, "\r\r")) != NULL || (redirect = strstr(buf, "\n\n")) != NULL )) {
        // Skip header
        redirect += 2;
        fprintf(stdout, "%s", redirect);
        fflush(stdout);
      } else {
        // Just print everything
        fprintf(stdout, "%s", buf);
        fflush(stdout);
      }
    } else {
      fprintf(stdout, "%s", buf);
      fflush(stdout);
    }
    is_first_pass = 0;
  }
  printf("\n");

  close(sockfd);
  if (urlinfo != NULL) {
    free(urlinfo);
  }

  return 0;
}
