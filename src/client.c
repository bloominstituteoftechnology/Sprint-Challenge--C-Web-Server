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
  char *hostname; /* the domain*/
  char *port; /*port number*/
  char *path; /*path on the domain*/
} urlinfo_t;

/**
 * Tokenize the given URL into hostname, path, and port.
 *
 * url: The input URL to parse.
 *
 * Store hostname, path, and port in a urlinfo_t struct and return the struct.
*/

/*this returns urlinfo_t struct*/
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

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  // The protocol ("http")
  // The server name ("www.google.com")
  // The file name ("web-server.htm")

  /*strchar is to find certain characters*/

  // we only want to try to get the address with "http://"
  char *temp = strstr(hostname, "://"); /*to look for a string*/

  if (temp != NULL) {
    hostname = temp + 3; /*this is to move after the colon and two slashes*/
  }
  // need someway to look for the path
  temp = strchr(hostname, '/');
  path = temp + 1;
  *temp = '\0'; 

  // we need to look for the port number 
  temp = strchr(hostname, ':');
  if (temp == NULL) {
    port = "80"; /*port 80 is the port that the server "listens to" or expects to receive from a Web client */
  } else {
    port = temp + 1;
    *temp = '\0';
  }

  urlinfo->hostname = hostname;
  urlinfo->port = port;
  urlinfo->path = path;

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

// this is almost the same as the homework!
int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

  int request_length = sprintf(request,
  /*this is going to be the path*/
    "GET /%s HTTP/1.1\n"
    "Host: %s:%s\n" /*typo here*/
    "Connection: close\n"
    "\n", /*we need this new line to know that its and of a response*/
    path,
    hostname,
    port
  );
  // sending it down// call send // do some error checking //  
  if ((rv = send(fd, request, request_length, 0) < 0)) {
    fprintf(stderr, "client: send");
    exit(3);
  }
// rv = return value
  return rv;
}

int main(int argc, char *argv[])
{  
  /*socket file discripter*/ 
  int sockfd, numbytes;  
  /*buffer*/
  char buf[BUFSIZE];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  /*
    1. Parse the input URL call parse_url.
    2. Initialize a socket by calling the `get_socket` function from lib.c
    3. Call `send_request` to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////

/*pass it the one argument that is pass to our client*/

/*the info_t is what we will get back*/
urlinfo_t *urlinfo = parse_url(argv[1]);
/*get_sockets requires a hostname and port*/
sockfd = get_socket(urlinfo->hostname, urlinfo->port);
// printf("REQUEST: host %s, port $s, path %s\n", urlinfo->hostname, urlinfo->port, urlinfo->path );
send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);
/*use the loop from README*/
   while ((numbytes = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0) {
       // print the data we got back to stdout
       fwrite(buf, 1, numbytes, stdout);
     }
    if (numbytes < 0) {
      fprintf(stderr, "client: recieve");
      exit(2);
    }
      printf("\n");

      free(urlinfo); /*don't forget to free it*/
      close(sockfd);

  return 0;
}

// https://en.wikibooks.org/wiki/C_Programming/string.h/strstr
// https://clc-wiki.net/wiki/strrchr
// https://searchnetworking.techtarget.com/definition/TCP
// https://medium.com/@vienchitang/what-is-an-lru-cache-3e8ad1853584
// https://medium.com/@vienchitang/what-is-an-lru-cache-3e8ad1853584
// https://searchnetworking.techtarget.com/definition/UDP-User-Datagram-Protocol
// https://computer.howstuffworks.com/web-server2.htm
// https://www.ibm.com/support/knowledgecenter/en/SS6PEW_9.4.0/com.ibm.help.custom.restapis.doc/c_SpecifyingHTTPHeaders.html
// https://stackoverflow.com/questions/2092527/what-happens-when-you-type-in-a-url-in-browser/2092602#2092602
// https://en.wikipedia.org/wiki/Server_(computing)
// https://www.sqa.org.uk/e-learning/HardOSSupp01CD/page_19.htm
// https://searchnetworking.techtarget.com/definition/port-80
// http://pubs.opengroup.org/onlinepubs/009696899/functions/send.html