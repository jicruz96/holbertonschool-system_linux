#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <netinet/in.h>

/**
 * struct todo_s - todo struct
 * @id: todo id
 * @title: title
 * @description: description
 * @repr: repr
 */
typedef struct todo_s
{
	int id;
	char *title;
	char *description;
	char *repr;
} todo_t;

void error_out(char *str);
void take_requests(int sockid);
void print_path_and_queries(char *buffer);
void print_headers(char *buffer);
void print_body_params(char *buffer);
int eval_request(char *buffer, int sockid, int client_id);
char **get_params(char *body);
char *get_param(char **params, char *key);
void add_todo(todo_t *todos, int id, char *title, char *description);
char *make_repr(int id, char *title, char *description);
int post(char *body, int id, int client_id, int sockid, todo_t *todos);



#endif /* _SOCKETS_H_ */
