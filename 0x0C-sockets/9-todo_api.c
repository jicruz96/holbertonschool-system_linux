#include "sockets.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

/**
 * main - REST API
 *        The program opens an IPv4/TCP socket and listens to traffic on port
 *        8080.
 * ----------------------------------------------------------------------------
 * GET /todos
 *     - Description:      Retrieves list of all todos
 *     - Required queries: None
 *     - Required headers: None
 *     - Required body parameters: None
 *     - Response: 200 OK
 *         - Response includes:
 *             header `Content-Type: application/json`
 *             json representation of the todos in its body
 * ----------------------------------------------------------------------------
 * POST /todos
 *     - Description:      Creates a todo and adds it to the list
 *     - Required queries: None
 *     - Required headers: `Content-Length`
 *     - Required body parameters:
 *         `title` -> String
 *         `description` -> String
 *         ! NOTE: cannot handle special URL-encoded characters
 *     - Response:
 *         201 Created -> Todo has been created.
 *             - Response includes:
 *                 header `Content-Type: application/json`
 *                 json representation of the created todo in its body
 *         411 Length Required -> Missing the Content-Length header
 *         422 Unprocessable Entity -> Missing a required body parameter
 * ----------------------------------------------------------------------------
 *
 * All other paths/methods should return `404 Not Found`
 *
 * Return: always zero
 */
int main(void)
{
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addrport;

	if (sockid == -1)
		error_out("Socket");
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(PORT);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);


	if (bind(sockid, (struct sockaddr *)&addrport, sizeof(addrport)) == -1)
		close(sockid), error_out("Bind");

	if (listen(sockid, 1) == -1)
		close(sockid), error_out("Listen");
	setbuf(stdout, NULL);
	printf("Server listening on port %d\n", PORT);
	take_requests(sockid);
	close(sockid);
	return (0);
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
