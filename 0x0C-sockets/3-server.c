#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT         12345

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
 * main - Opens an IPv4/TCP socket and listens to traffic on port 12345. Can
 *        accept an entering connection and waits for an incoming message.
 *        It prints the received message, then closes the connection
 *
 * Return: always zero
 */
int main(void)
{
	int client_id, server_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	socklen_t addr_size = sizeof(struct sockaddr);
	struct sockaddr_in server_addr, client_addr;
	char buffer[1024];


	if (server_id == -1)
		error_out("Socket", NULL, NULL);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_id, (struct sockaddr *)&server_addr, addr_size) == -1)
		error_out("Bind", &server_id, NULL);

	if (listen(server_id, 1) == -1)
		error_out("Listen", &server_id, NULL);

	printf("Server listening on port %d\n", PORT);

	client_id = accept(server_id, (struct sockaddr *)&client_addr, &addr_size);
	if (client_id == -1)
		error_out("Accept", &server_id, NULL);

	printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

	if (recv(client_id, buffer, sizeof(buffer), 0) == -1)
		error_out("Recv", &server_id, &client_id);

	printf("Message received: \"%s\"\n", buffer);

	close(client_id);
	close(server_id);
	return (0);
}
