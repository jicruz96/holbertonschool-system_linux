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
