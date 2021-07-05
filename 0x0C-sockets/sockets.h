#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <netinet/in.h>

void error_out(char *str);
void take_requests(int sockid);
void print_path_and_queries(char *buffer);
void print_headers(char *buffer);
void print_body_params(char *buffer);
int eval_request(char *buffer, int sockid, int client_id);



#endif /* _SOCKETS_H_ */
