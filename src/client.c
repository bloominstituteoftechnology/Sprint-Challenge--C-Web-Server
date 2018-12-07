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
urlinfo_t *actual_parse_logic (char *url)
{
  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));
  // printf("no http or https:// \n");

    int i, j, k, length; 
    // i = begining  j = :    k = /
    i = 0, j = 0, k = 0, length = 0; 
    char colon = ':';
    char backslash = '/';

    while (url[length] != '\0'){
      // printf("%c\n", full_site[length]);
      if(colon == url[length]){
        // printf("equal 0\n");
        j = length; 
      } else if (url[length] == '/'){
        // printf("equal 0\n");
        k = length; 
      }

      length++; 

    }  

    printf("the numbers i j k length %d |  %d |  %d | %d \n", i, j, k, length); 


    char test[j-i+1]; //hostname
    char test1[k-j]; //port 
    char test2[length -j+1]; //path  
    
    
    strncpy(test, url+i, j-i); // begining to colon. 
    test[j] = '\0'; 
    // printf("should be the host name:    %s\n", test);
    //AT this position localhost is the hostname  however  by the time it gets down the main function it changes. 

    j++;// to remove the colon. 
    strncpy(test1, url+j, k-j); //colon to slash 
    test1[k-j] = '\0'; 
    k++; // remove the back slash. 

    strncpy(test2, url+k, length-j);
    test2[length-j] = '\0'; 

    // printf("should be the host name:    %s\n", test); 
    // printf("should be the port : %s\n", test1);
    // printf("should be the path :  %s\n", test2); 
    
    urlinfo->hostname = strdup(test); 
    // printf("should be the host name:    %s\n", test);
    urlinfo->port = strdup(test1); 
    urlinfo->path = strdup(test2); 

    // printf("%s, %s, %s", urlinfo->hostname, urlinfo->port, urlinfo->path);
    //may have to malloc space for this 
    // using strdup  allowed me to get the full hostname  instead of a cut off version. 
    return urlinfo;

  }

  



urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  // char *hostname = strdup(url);
  // char *port;
  // char *path;

  

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

  char *temp; 

  temp = strstr(url, "//"); 
  // printf("%s  temp\n", temp); 
  if (temp != NULL){
    temp++;//removes the back slash
    temp++;//removes the back slash
    // printf("%s  temp\n", temp);
  }
  if(temp == NULL){//if there is not // in the url 
   return  actual_parse_logic(url);
} else {//if there is a // in the url use temp. 
  return actual_parse_logic(temp); 
  }

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

  int request_length = sprintf(request, "GET /%s HTTP/1.1\n"
  "Host: %s: %s\n"
  "Connection: close",  path, hostname, port 
  );


  rv = send(fd, request,request_length, 0);

  if (rv < 0){
    perror("send");
    exit(1);
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
    2. Initialize a socket
    3. Call send_request to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  urlinfo_t *urlinfo = parse_url(argv[1]);
  // printf("%s %s %s\n", urlinfo->hostname, urlinfo->port, urlinfo->path);
  sockfd = get_socket(urlinfo->hostname, urlinfo->port); 
  int rv = send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path); 

  //recv 

  while ((numbytes = recv(sockfd, buf, BUFSIZ -1, 0) > 0 )) {
    //print the data we got back to stdout 
    printf("%s\n", buf); 
  } 

  /*Performing tests for parse url */
  // urlinfo_t *urlinfo = parse_url("http://localhost:3490/d20");   tested and works 
  // urlinfo_t *urlinfo = parse_url("localhost:3490/d20");  tested and works. 

  // printf("%s %s %s\n", urlinfo->hostname, urlinfo->port, urlinfo->path);  only for testing
  return 0;

}
