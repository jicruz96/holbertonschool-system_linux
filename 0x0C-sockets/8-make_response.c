#include "sockets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "http_request_parser.c"
#include "todos.c"


#ifdef TODO_API_5
#define GET_URIS     {"/todos"}
#else
#define GET_URIS     {NULL}
#endif

#define HEAD_URIS    {NULL}
#define POST_URIS    {"/todos"}
#define PUT_URIS     {NULL}

#ifdef TODO_API_7
#define DELETE_URIS  {"/todos"}
#else
#define DELETE_URIS  {NULL}
#endif

#define CONNECT_URIS {NULL}
#define OPTIONS_URIS {NULL}
#define TRACE_URIS   {NULL}


/**
 * known_uri - returns true if the request is making a valid request
 *
 * @request: pointer to http_request_t struct describing request
 * Return: true if request is valid, false if not
 */
int known_uri(http_request_t *request)
{
	char *uris_by_method[][NUM_HTTP_METHODS] = {
		GET_URIS,
		HEAD_URIS,
		POST_URIS,
		PUT_URIS,
		DELETE_URIS,
		CONNECT_URIS,
		OPTIONS_URIS,
		TRACE_URIS
	};
	char **uris = uris_by_method[request->method];
	size_t i;


	for (i = 0; uris[i]; i++)
		if (!strcmp(request->uri, uris[i]))
			return (true);

	return (false);
}


/**
 * process_get_request - processes a get request
 * @request: pointer to struct describing request
 * @body: pointer to response buffer
 * @todos: list of todos
 * @sum_repr_lens: length of the sum of all representations
 * @id: max id
 * Return: size of response
 */
size_t process_get_request(http_request_t *request, char **body, todo_t *todos,
int sum_repr_lens, int id)
{
	char *delim, *tmp = get_param(request->query_params, "id");
	size_t length;
	int i;

	if (tmp)
	{
		i = atoi(tmp);
		if (i >= id || !todos[i].repr)
			return (0);
		*body = strdup(todos[i].repr);
		length = todos[i].repr_len;
	}
	else
	{
		*body = malloc(sizeof(char) * (sum_repr_lens + 3));
		**body = '[';
		for (i = 0, length = 1, delim = ""; i < id; i++)
			if (todos[i].repr)
			{
				length += sprintf(*body + length, "%s%s", delim, todos[i].repr);
				delim = ",";
			}
		(*body)[length++] = ']';
		(*body)[length] = '\0';
	}

	return (length);
}

/**
 * process_delete_request - processes a delete request
 * @request: pointer to struct that describes request
 * @todos: pointer to link list of todo items
 * @id: max id value allowed
 * Return: "\r\n" on success and NULL on failure
 */
char *process_delete_request(http_request_t *request, 
todo_t *todos, int id, int *sum_repr_lens)
{
	char *tmp = get_param(request->query_params, "id");
	int i;

	if (!tmp)
		return (NULL);
	
	i = atoi(tmp);

	if (i >= id || !todos[i].repr)
		return (NULL);
	
	*sum_repr_lens -= todos[i].repr_len;
	
	memset(&todos[i], 0, sizeof(todo_t));
	return (strdup("\r\n"));
}

/**
 * process_request - processes a request
 *
 * @request: pointer to request
 * Return: response to send back
 */
char *process_request(http_request_t *request)
{
	char *title, *description, *response, *body = NULL;
	static todo_t todos[100];
	static int id, sum_repr_lens;
	size_t length;

#ifdef TODO_API_7
	if (request->method == DELETE)
		return (process_delete_request(request, todos, id, &sum_repr_lens));
#endif

	if (request->method == GET)
	{
		length = process_get_request(request, &body, todos, sum_repr_lens, id);
	}
	else
	{
		title = get_param(request->body_params, "title");
		description = get_param(request->body_params, "description");

		if (!title || !description)
			return (NULL);

		add_todo(todos, id, title, description);
		sum_repr_lens += todos[id].repr_len;
		body = strdup(todos[id].repr);
		length = todos[id].repr_len;
		id++;
	}

#define RES_STR \
"Content-Length: %lu\r\n\
Content-Type: application/json\r\n\
\r\n\
%s" /* body goes here */
	if (!body)
		return (NULL);
	response = calloc(length + sizeof(RES_STR) + 10, sizeof(char));
	sprintf(response, RES_STR, length, body);
	free(body);
	return (response);
}

/**
 * make_response - responds to a request
 * @client_address: client address (ignored)
 * @buffer: buffer where client's request is stored.
 * Return: response
 */
char *make_response(char *client_address, char *buffer)
{
	char *method, *uri, *status, *res, *response = NULL;
	struct http_request_s *request;

	(void)client_address;

	if (!(request = http_request_init(buffer)))
		status = "400 Bad Request";
	else if (!known_uri(request))
		status = "404 Not Found";
	else if (
		request->method == POST &&
		!get_header(request->headers, "Content-Length")
	)
		status = "411 Length Required";
	else if (!(response = process_request(request)))
		if (request->method == POST)
			status = "422 Unprocessable Entity";
		else
			status = "404 Not Found";
	else if (request->method == POST)
		status = "201 Created";
	else if (request->method == DELETE)
		status = "204 No Content";
	else
		status = "200 OK";

	method   = request  ? request->method_str : strtok(buffer, " ");
	uri      = request  ? request->uri    : strtok(NULL, "? ");
	printf("%s %s -> %s\n", method, uri, status);
	res = calloc(1024, sizeof(char));
	sprintf(res, "HTTP/1.1 %s\r\n%s", status, response ? response : "\r\n");
	free_http_request(request);
	free(response);
	return (res);
}
