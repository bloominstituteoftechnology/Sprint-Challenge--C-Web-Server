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

char *replace_w_space(char *needs_space){
  int cnt = 0;
  while(needs_space[cnt] != "\0"){
    if(needs_space[cnt] == "/" || needs_space[cnt] == "/" ){
      needs_space[cnt] = " ";
    } 
    cnt++;
  }
  return needs_space;
}

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
  char *temp_url[128];
  char *point_break;

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

  if(strstr(hostname, "http://" == 0)){
    strncpy(temp_url, hostname+7, sizeof(temp_url));
    printf("tEMPURL >>>> %s", temp_url);
    point_break = replace_w_space(temp_url);
    sscanf(point_break, "%s %s %s", hostname, port, path);
    printf("hostname >> %s, port >>> %s, path >> %s\n", hostname, port, path);
  } else if (strstr(hostname, "https://") == 0) {
    strncpy(temp_url, hostname+8, sizeof(temp_url));
    printf("tEMPURL >>>> %s", temp_url);
    point_break = replace_w_space(temp_url);
    sscanf(point_break, "%s %s %s", hostname, port, path);
    printf("hostname >> %s, port >>> %s, path >> %s\n", hostname, port, path);
  } else {
    point_break = replace_w_space(temp_url);
    sscanf(point_break, "%s %s %s", hostname, port, path);
    printf("hostname >> %s, port >>> %s, path >> %s\n", hostname, port, path);
  }

  urlinfo->hostname = strdup(hostname);
  urlinfo->path = strdup(path);
  urlinfo->port = strdup(port);

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
  

  return 0;
}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];
  char *URL = argv[1];

  // 1. Parse the input URL
  struct urlinfo_t *urlinfo = parse_url(URL);

  // 2. Initialize socket
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);

  // 3. Call send_request to construct the request and send it
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);

  // 4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.

  while((numbytes = recv(sockfd, buf, BUFSIZE - numbytes, 0))) {
    //  
  }


  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }


    // 5. Clean up any allocated memory and open file descriptors.
  

  return 0;
}
