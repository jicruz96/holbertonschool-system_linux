#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <netinet/in.h>

void error_out(char *str);
void take_requests(int sockid);
void print_path_and_queries(char *line);



#endif /* _SOCKETS_H_ */
