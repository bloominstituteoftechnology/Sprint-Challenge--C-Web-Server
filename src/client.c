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
    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

  // // Use strchr to find the first backslash in the URL
  // path = strchr(url, '/');
  // // Overwrite the backslash with a '\0'
  // *path = '\0';  
  // // Set the path pointer to 1 character after
  // path++;

  // // Use strchr to find the first colon in the URL
  // port = strchr(url, ':');
  // // Overwrite the colon with a '\0'
  // *port = '\0';
  // // Set the port pointer to 1 character after
  // port++;

  // Use temp and strstr to find the first backslash in the URL
  char *temp = strstr(hostname, "/");
  // Overwrite the backslash with a '\0'
  *temp = '\0';
  // Set the path pointer to 1 character after
  path = temp + 1;

  // Use temp and strstr to find the first colon in the URL
  temp = strstr(hostname, ":");
  // Overwrite the colon with a '\0'
  *temp = '\0';
  // Set the port pointer to 1 character after
  port = temp + 1;

  // Store hostname, path, and port in a urlinfo_t struct (named urlinfo above) 
  urlinfo->path = path;
  urlinfo->port = port;
  urlinfo->hostname = hostname;

  // and return the struct.
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
 * Requests should look like the following:
   GET /path HTTP/1.1
   Host: hostname:port
   Connection: close
 *
 * Return the value from the send() function.
*/
int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];

  // copied over response_length from server.c
  // change instances of response to request
  // followed what requests should look like above
  // sent it all using rv from server.c!

  int request_length = sprintf(request,
        
        // added missing \n seperators
        "GET /%s HTTP/1.1\n"
        "Host: %s:%s\n"
        "Connection: close\n",

        path,
        hostname,
        port
    );

  int rv = send(fd, request, request_length, 0); // remove int because it's already defined

  if (rv < 0) {
      perror("send");
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
    2. Call the `get_socket()` function in order to initialize a socket 
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server:
      while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0) {
      // print the data we got back to stdout
      }
    5. Print the received response to stdout.
    6. Clean up any allocated memory and open file descriptors.
  */

  // parse the input URL
  urlinfo_t *urlinfo = parse_url(argv[1]);
  // initialize a socket with `get_socket()`
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  // send_request
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);
  // call `recv` in a loop
  while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0) {
    // print the data we got back to stdout
    printf("%s\n", buf);
  }

  close(sockfd);
  free(urlinfo);

  return 0;
}

// Your client should print out the correct response to `stdout`, something like:
// ```
// HTTP/1.1 200 OK
// Date: Tue Oct  2 11:41:43 2018
// Connection: close
// Content-Length: 3
// Content-Type: text/plain

// 17

// ```

// executing `./client www.google.com:80/` should return back a 200 status code with all the HTML that makes up Google's homepage and print it all to `stdout`. The response header will look something like this:
// ```
// HTTP/1.1 200 OK
// Date: Tue, 02 Oct 2018 18:44:13 GMT
// Expires: -1
// Cache-Control: private, max-age=0
// Content-Type: text/html; charset=ISO-8859-1
// P3P: CP="This is not a P3P policy! See g.co/p3phelp for more info."
// Server: gws
// X-XSS-Protection: 1; mode=block
// X-Frame-Options: SAMEORIGIN
// Set-Cookie: 1P_JAR=2018-10-02-18; expires=Thu, 01-Nov-2018 18:44:13 GMT; path=/; domain=.google.com
// Set-Cookie: NID=140=xQnQZhdVuKxdbMlSwuwPo-3Ii375x3h2c936Kcyk_JA8HAZTunEFW2L5F93UcSqDI-JtnHgl3r_qwZVxyJMFvMKYDKYZf4ab25QjziB5iFRNuNpjDEPKa8bn7ICeWNsH; expires=Wed, 03-Apr-2019 18:44:13 GMT; path=/; domain=.google.com; HttpOnly
// Accept-Ranges: none
// Vary: Accept-Encoding
// Connection: close

// ```