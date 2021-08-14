#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

char *make_response(char *address, char *request);

/**
 * error_out - prints error, closes open file descriptors and exits
 *
 * @str: error message
 * @server_id: server socket file descriptor
 * @client_id: client socket file descriptor
 */
static void error_out(char *str, int *server_id, int *client_id)
{
	perror(str);
	if (client_id)
		close(*client_id);
	if (server_id)
		close(*server_id);
	exit(EXIT_FAILURE);
}

/**
 * take_requests - accepts new connections, responds
 * @server_id: server socket file descriptor
 *
 */
void take_requests(int server_id)
{
	int client_id;
	struct sockaddr_in client_addr;
	socklen_t addr_size = sizeof(struct sockaddr);
	char request[1024] = {0}, *response;

	while (1)
	{
		client_id = accept(server_id, (struct sockaddr *)&client_addr, &addr_size);
		if (client_id == -1)
			error_out("Accept", &server_id, NULL);


		if (recv(client_id, request, sizeof(request), 0) == -1)
			error_out("recv", &server_id, &client_id);

		response = make_response(inet_ntoa(client_addr.sin_addr), request);

		if (send(client_id, response, strlen(response), 0) == -1)
			free(response), error_out("send", &server_id, &client_id);

		close(client_id);
		free(response);
		memset(request, '\0', sizeof(request));
	}
}

/**
 * main - REST API
 *        The program opens an IPv4/TCP socket and listens to traffic on port
 *        8080.
 * ----------------------------------------------------------------------------
 *     GET /todos?id={id}
 *     - Description: Retrieves a single todo item, identified by its id
 *     - Required queries:
 *         `id` -> uint
 *     - Required headers: None
 *     - Required body parameters: None
 *     - Response:
 *         200 OK -> Todo retrieved
 *             - Response includes:
 *                 header `Content-Type: application/json`
 *                 json representation of the todo in its body
 *         404 Not Found -> Unrecognized id
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
	int server_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;

	if (server_id == -1)
		error_out("Socket", NULL, NULL);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);


	if (bind(server_id, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		error_out("Bind", &server_id, NULL);

	if (listen(server_id, 1) == -1)
		error_out("Listen", &server_id, NULL);

	setbuf(stdout, NULL);
	printf("Server listening on port %d\n", PORT);
	take_requests(server_id);
	close(server_id);
	return (0);
}
