#include "sockets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


char *make_response(char *body, char *status);

/**
 * eval_request - parses a request for info, returns status code
 *
 * @buffer: buffer containing request
 * @sockid: sockid
 * @client_id: client id
 * Return: status code
 */
int eval_request(char *buffer, int sockid, int client_id)
{
	char *method = strtok(buffer, " "), *path = strtok(NULL, " "), res[1024];
	char *response, *query = strchr(path, '?');
	int i, len, status;
	static int id;
	static todo_t todos[1024];

	if (query)
		*(query++) = '\0';
	printf("%s %s -> ", method, path);
	if ((strcmp(method, "POST") && strcmp(method, "GET")) ||
		strcmp(path, "/todos"))
	{
		printf("404 Not Found\n");
		return (404);
	}
	if (strcmp(method, "POST") == 0)
	{
		status = post(strtok(NULL, "\n"), id, client_id, sockid, todos);
		if (status == 201)
			id++;
		return (status);
	}

	if (query && !strncmp(query, "id=", 3))
	{
		i = atoi(query + 3);
		if (i >= id)
		{
			printf("404 Not Found\n");
			return (404);
		}
		response = make_response(todos[i].repr, "HTTP/1.1 200 OK\r\n");
	}
	else
	{
		res[0] = '[';
		len = 1;
		for (i = 0; i < id; i++)
			len += sprintf(res + len, "%s%s", i ? "," : "", todos[i].repr);
		res[len++] = ']';
		res[len]   = '\0';
		response = make_response(res, "HTTP/1.1 200 OK\r\n");
	}


	if (send(client_id, response, strlen(response), 0) == -1)
		close(sockid), close(client_id), error_out("send");
	printf("200 OK\n");
	return (200);
}

/**
 * post - posts a request
 *
 * @body: body
 * @id: id
 * @client_id: client_id
 * @sockid: sockid
 * @todos: todos
 * Return: status code
 */
int post(char *body, int id, int client_id, int sockid, todo_t *todos)
{
	char *repr, *header, *title, *description, *response, **params;
	int has_length = 0;


	while (!has_length && *body != '\r')
	{
		header = strtok(NULL, ":");
		has_length |= (strcmp(header, "Content-Length") == 0);
		body = strtok(NULL, "\n");
	}

	if (!has_length)
	{
		printf("411 Length Required\n");
		return (411);
	}

	while (*body != '\r')
		body = strtok(NULL, "\n");
	params = get_params(strtok(NULL, "\n"));
	title = get_param(params, "title");
	description = get_param(params, "description");
	if (!title || !description)
	{
		printf("422 Unprocessable Entity\n");
		return (422);
	}
	printf("201 Created\n");
	repr = make_repr(id, title, description);
	response = make_response(repr, "HTTP/1.1 201 Created\r\n");
	free(repr);
	add_todo(todos, id, title, description);
	if (send(client_id, response, strlen(response), 0) == -1)
	{
		free(params), free(response);
		close(sockid), close(client_id), error_out("send");
	}
	free(params);
	free(response);
	return (201);
}

/**
 * add_todo - adds todo item
 *
 * @todos: todos
 * @id: id
 * @title: title
 * @description: description
 */
void add_todo(todo_t *todos, int id, char *title, char *description)
{
	todo_t *todo = todos + id;

	todo->id = id;
	todo->title = strdup(title);
	todo->description = strdup(description);
	todo->repr = make_repr(id, title, description);
}

/**
 * make_repr - makes repr
 *
 * @id: id
 * @title: title
 * @description: description
 * Return: repr
 */
char *make_repr(int id, char *title, char *description)
{
#define DICT     "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}"
	char *repr, id_str[20];
	size_t len;

	sprintf(id_str, "%d", id);
	len = strlen(id_str) + strlen(title) + strlen(description) + strlen(DICT);
	repr = malloc(sizeof(char) * (len + 1));
	if (repr)
		sprintf(repr, DICT, id, title, description);
	return (repr);
}



/**
 * make_response - makes response
 * @body: response body
 * @status: response status
 * Return: response string
 */
char *make_response(char *body, char *status)
{
	char *response;
	size_t len;
#define CON_LEN  "Content-Length: "
#define CON_TYPE "Content-Type: application/json\r\n\r\n"
	response = calloc(1024, sizeof(char));
	if (!response)
		return (NULL);
	len = strlen(body);
	sprintf(response, "%s%s%lu\r\n%s%s", status, CON_LEN, len, CON_TYPE, body);
	return (response);
}
