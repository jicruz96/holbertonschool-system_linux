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
