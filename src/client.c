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

   * Your client should be able to handle URLs such as `localhost:3490/d20` and
     `www.google.com:80/`. Input URLs need to be broken down into `hostname`,
     `port`, and `path`. The `hostname` is everything before the colon (but
     doesn't include `http://` or `https://` if either are present), the `port`
     is the number after the colon ending at the backslash, and the `path` is
     everything after the backslash.



*/
urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *path;
  char backslash = '\\';
  char colon = ':';
  path = strchr(hostname, backslash)
  backslash = '\0';
  port = strchr(hostname, colon);
  colon = '\0';

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));
  urlinfo_t->hostname = hostname;
  urlinfo_t->port = port;
  urlinfo_t->path = path;
  return urlinfo_t;
  // TODO
  //  * Implement the `parse_url()` function, which receives the input URL and tokenizes it into `hostname`, `port`, and `path` strings. Assign each of these to the appropriate field in the `urlinfo_t` struct and return it from the `parse_url()` function.
  /*

    // sscanf(request, "%s %s", method, path);
    // printf("Method, path: %s %s\n", method, path);
    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    char *strchr(const char *str, int c)
    #include <stdio.h>
#include <string.h>

int main () {
   const char str[] = "http://www.tutorialspoint.com";
   const char ch = '.';
   char *ret;

   ret = strchr(str, ch);

   printf("String after |%c| is - |%s|\n", ch, ret);
   
   return(0);
}
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

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
int send_response(int fd, char *header, char *content_type, void *body, int content_length)
{
    const int max_response_size = 262144;
    char response[max_response_size];


    // Remove Body so it can be sent separately from the header (uses 2 send())
    snprintf(response, max_response_size,
        "%s\n" 
        "Content-Type: %s\n"
        "Content-Length: %d\n" 
        "Connection: close\n"
        "\n",
        header, content_type, content_length
    );

    printf("%s", response); // send()

    int response_length = strlen(response);

    // Send it all!
    int rv = send(fd, response, response_length, 0);

    if (rv < 0) {
        perror("send");
    }

    rv = send(fd, body, content_length, 0);

    if (rv < 0) {
        perror("send");
    }

    return rv;
    }

*/
int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

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
    2. Initialize a socket by calling the `get_socket` function from lib.c
    3. Call `send_request` to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  return 0;
}
