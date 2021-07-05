#include "sockets.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

/**
 * main - REST API - POST request example
 *        The program opens an IPv4/TCP socket and listens to traffic on port
 *        8080.
 * Method:           POST
 * Path:             /todos
 * Description:      Creates a todo and adds it to the list
 * Required queries: None
 * Required headers: `Content-Length`
 * Required body parameters:
 *     `title` -> String
 *     `description` -> String
 *     ! NOTE: cannot handle special URL-encoded characters
 * Response:
 *     201 Created -> Todo has been created.
 *         Response includes:
 *             header `Content-Type: application/json`
 *             json representation of the created todo in its body
 *     411 Length Required -> Missing the Content-Length header
 *     422 Unprocessable Entity -> Missing a required body parameter
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
	char *method = strtok(buffer, " ");
	char *path = strtok(NULL, " ");
	char *header, *body, *title, *description, response[1024];
	int has_length = 0;
	static int id;

	printf("%s %s -> ", method, path);

	if (strcmp(method, "POST") || strcmp(path, "/todos"))
	{
		printf("404 Not Found\n");
		return (404);
	}

	body = strtok(NULL, "\n");
	while (!has_length && *body != '\r')
	{
		header = strtok(NULL, ":");
		has_length = (strcmp(header, "Content-Length") == 0);
		body = strtok(NULL, "\n");
	}

	if (!has_length)
	{
		printf("411 Length Required\n");
		return (411);
	}
	while (*body != '\r')
		body = strtok(NULL, "\n");
	body = strtok(NULL, "\n");
	title = strtok(NULL, "&");
	description = strtok(NULL, "\r");
	if (!body ||
		!title || strncmp(title, "title=", 6) ||
		!description || strncmp(description, "description=", 12))
	{
		printf("422 Unprocessable Entity\n", title);
		return (422);
	}

	printf("201 Created\n");
#define DICT "{\"id\": %d, \"title\": \"%s\", \"description\": \"%s\"}"
	sprintf(response, DICT, id++, title + 6, description + 12);
	if (send(client_id, response, strlen(response), 0) == -1)
		close(sockid), close(client_id), error_out("send");

	return (0);


}

/**
 * print_body_params - helper for take_requests()
 *
 * @buffer: buffer containing client request
 */
void print_body_params(char *buffer)
{
	char *body_params = strstr(buffer, "\r\n\r\n") + 4;
	char *path = strtok(strtok(strchr(buffer, ' ') + 1, " "), "?");
	char *key, *value;

	printf("Path: %s\n", path);

	for (
		key = strtok(body_params, "="), value = strtok(NULL, "&");
		key && value;
		key = strtok(NULL, "="), value = strtok(NULL, "&")
	)
		printf("Body param: \"%s\" -> \"%s\"\n", key, value);
}
