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

  printf("Inside parse_url method URL is %s\n", url);
  /*
    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

	char *first_backslash = strchr(hostname, '/');

	//first_backslash++;
	path = first_backslash+1;
	
	printf("path is %s\n", path);
	*first_backslash = '\0';
	
	// example localhost:3490/d20

	char *first_colon = strchr(hostname, ':');
	port = first_colon+1;

	*first_colon = '\0';
	printf("port is %s\n", port);


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

  char *URL;  

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



	//1. check if there is http:// or https:// in the URL
	char *check_1 = strstr(argv[1], "http://");

	char *check_2 = strstr(argv[1], "https://");
	
	if(check_1){
		printf("http present\n");
		check_1+=7;
		URL = check_1;

		printf("URL: %s\n", URL);
		
	}
	else if(check_2){
		printf("https present\n");
		check_2+=8;
		URL = check_2;
		
		printf("URL: %s\n", URL);
	}
	else{
		 printf("No http or https present\n");
		 URL = argv[1];
		 printf("URL: %s\n", URL);
	}	

	//making a call to parse_url() to get hostname, port
	urlinfo_t *url_info = parse_url(URL);
	

  	//2 Initialize a socket by calling the get_socket()
	printf("url_info->hostname is %s ans url_info->port is %s\n", url_info->hostname, url_info->port);
	sockfd = get_socket(url_info->hostname, url_info->port);
	printf("socket_id is %d\n", sockfd);




  return 0;
}
