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

    1. Use strchr to find the first slash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the slash with a '\0' so that we are no longer considering anything after the slash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  path = strchr(hostname, '/');
  //Todo error check if path is NULL
  *path = '\0';
  path++;

  port = strchr(hostname, ':');
  if(port==NULL){
    port == "80";
  }else{
  *port = '\0';
  port++;
  }
  //Todo error check if port is NULL
  
  urlinfo->port = port;
  urlinfo->hostname = hostname;
  urlinfo->path = path;

  return urlinfo;
  // parse_url(argv[1])
  // const char str[] = "www.google.com/";
  //  const char ch = '.';
  //  char *ret;

  //  ret = strchr(str, ch);

  //  printf("String after |%c| is - |%s|\n", ch, ret);
   
  //  return(0);

  // return urlinfo;
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
    "%s\nfd: %s\nhostname: %s\nport: %s\npath: %s\n",
    hostname,
    port,
    path
  );
 int rv = send(fd, request, request_length, 0);

    if (rv < 0)
    {
        perror("send");
    }

    return rv;
  //return 0;
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

urlinfo_t* urlinfo = parse_url(argv[1]);

  printf("Hostname: \"%s\"\n", urlinfo->hostname);
  printf("Port: \"%s\"\n",urlinfo->port);
  printf("Path: \"%s\"\n",urlinfo->path);
  int fd = get_socket(urlinfo->hostname, urlinfo->port);

  if (fd == -1){
    perror("get_socket");
    exit(2);
  };

send_request(fd, urlinfo->hostname, urlinfo->port, urlinfo->path);

while ((numbytes = recv(fd,buf,BUFSIZE, 0))>0){
  fwrite(buf, sizeof(char),numbytes, stdout);
}

if(numbytes < 0){
  perror("recv");
}

  return 0;
}
