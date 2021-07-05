#include "sockets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DICT "{\"id\": %d, \"title\": \"%s\", \"description\": \"%s\"}"

/**
 * error_out - prints error and exits
 *
 * @str: error message
 */
void error_out(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

/**
 * take_requests - accepts new connections, responds
 * @sockid: server socket file descriptor
 *
 */
void take_requests(int sockid)
{
	int status, client_id;
	struct sockaddr client_addr;
	socklen_t client_addr_size = sizeof(struct sockaddr);
	char *address, buffer[1024], *response;
	size_t response_size;

	while (1)
	{
		client_id = accept(sockid, &client_addr, &client_addr_size);
		if (client_id == -1)
			close(sockid), error_out("Accept");

		address = inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr);
		printf("%s ", address);

		memset(buffer, 0, sizeof(buffer));

		if (recv(client_id, buffer, sizeof(buffer), 0) == -1)
			close(sockid), close(client_id), error_out("recv");
		status = eval_request(buffer, sockid, client_id);
		if (status == 422)
			response = "HTTP/1.1 422 Unprocessable Entity\r\n\r\n";
		else if (status == 411)
			response = "HTTP/1.1 411 Length Required\r\n\r\n";
		else if (status == 404)
			response = "HTTP/1.1 404 Not Found\r\n\r\n";
		else
		{
			close(client_id);
			continue;
		}
		response_size = strlen(response);
		fflush(stdout);
		strcpy(buffer, response);

		if (send(client_id, buffer, response_size, 0) == -1)
			close(sockid), close(client_id), error_out("send");

		close(client_id);
	}
}

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
	char *method = strtok(buffer, " "), *path = strtok(NULL, " ");
	char **params, *header, *body, *title, *description, response[1024];
	int has_length = 0;
	static int id;

	printf("%s %s -> ", method, path);
	if (strcmp(method, "POST") || strncmp(path, "/todos", 6))
	{
		printf("404 Not Found\n");
		return (404);
	}
	body = strtok(NULL, "\n");
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
	sprintf(response, DICT, id++, title, description);
	if (send(client_id, response, strlen(response), 0) == -1)
		close(sockid), close(client_id), error_out("send");
	return (0);
}

/**
 * get_params - makes params list
 * @body: body of request
 * Return: params list
 */
char **get_params(char *body)
{
	int i, num_params = 0;
	char **params, *param, *body_params = strtok(body, "\r");

	if (!body_params)
		return (NULL);

	for (i = 0; body_params[i]; i++)
		if (body_params[i] == '=')
			num_params++;

	if (!num_params)
		return (NULL);

	params = malloc(sizeof(char *) * (num_params + 1));
	if (!params)
		return (NULL);
	params[num_params] = NULL;

	param = strtok(body_params, "&");
	for (i = 0; param; i++, param = strtok(NULL, "&"))
		params[i] = param;

	return (params);
}

/**
 * get_param - returns a parameter value from a list of parameters
 * @params: parameters
 * @key: key to search for
 * Return: value of key
 */
char *get_param(char **params, char *key)
{
	int i;
	size_t key_size = strlen(key);

	for (i = 0; params[i]; i++)
		if (strncmp(params[i], key, key_size) == 0)
			return (params[i] + key_size + 1);

	return (NULL);
}
