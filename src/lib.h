#ifndef _LIB_H_
#define _LIB_H_

void *get_in_addr(struct sockaddr *sa);
int get_socket(char *hostname, char *port);

#endif