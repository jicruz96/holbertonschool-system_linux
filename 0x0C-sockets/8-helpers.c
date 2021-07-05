#include "sockets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

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
	char **params, *header, *body, *title, *description, *response;
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
	response = make_response(id++, title, description);
	if (send(client_id, response, strlen(response), 0) == -1)
		close(sockid), close(client_id), error_out("send");
	free(params);
	free(response);
	return (0);
}

/**
 * make_response - makes response
 * @id: id
 * @title: title
 * @description: description
 * Return: response string
 */
char *make_response(int id, char *title, char *description)
{
	char id_str[10], dict[512];
	char *response;
	size_t len;
#define RES      "HTTP/1.1 201 Created\r\n"
#define CON_LEN  "Content-Length: "
#define CON_TYPE "Content-Type: application/json\r\n\r\n"
#define DICT     "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}"
	response = calloc(1024, sizeof(char));
	if (!response)
		return (NULL);
	sprintf(id_str, "%d", id);
	sprintf(dict, DICT, id, title, description);
	len = strlen(dict);
	sprintf(response, "%s%s%lu\r\n%s%s", RES, CON_LEN, len, CON_TYPE, dict);
	return (response);
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
