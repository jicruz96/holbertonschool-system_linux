#include "sockets.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080

/**
 * main - REST API - Queries
 *        The program does the following:
 *        1) Opens an IPv4/TCP socket and listens to traffic on port 8080.
 *        2) Accepts an entering connection, prints the IP address of the
 *           connected client, and waits for an incoming message.
 *        3) Prints:
 *               * the full received HTTP request
 *               * the request path
 *               * all query key/value pairs of the HTTP request
 *        4) Sends back a response to the connected client
 *        5) closes the connection
 *        6) waits for the next connection, and repeats steps 2-6
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
	int client_id;
	struct sockaddr client_addr;
	socklen_t client_addr_size = sizeof(struct sockaddr);
	char *address, buffer[1024], *response = "HTTP/1.1 200 OK\r\n\r\n";
	size_t response_size = strlen(response);

	while (1)
	{
		client_id = accept(sockid, &client_addr, &client_addr_size);
		if (client_id == -1)
			close(sockid), error_out("Accept");

		address = inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr);
		printf("Client connected: %s\n", address);

		memset(buffer, 0, sizeof(buffer));

		if (recv(client_id, buffer, sizeof(buffer), 0) == -1)
			close(sockid), close(client_id), error_out("recv");

		printf("Raw request: \"%s\"\n", buffer);
		print_path_and_queries(buffer);

		strcpy(buffer, response);

		if (send(client_id, buffer, response_size, 0) == -1)
			close(sockid), close(client_id), error_out("send");

		close(client_id);
	}
}

/**
 * print_path_and_queries - helper for take_requests()
 *
 * @buffer: buffer containing client request
 */
void print_path_and_queries(char *buffer)
{
	char *key, *value;

	printf("Path: %s\n", strtok(strtok(strchr(buffer, ' ') + 1, "/ "), "?"));

	for (
		key = strtok(NULL, "="), value = strtok(NULL, "&");
		key && value;
		key = strtok(NULL, "="), value = strtok(NULL, "&")
	)
		printf("Query: \"%s\" -> \"%s\"\n", key, value);
}
