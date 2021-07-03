#include "sockets.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 12345

/**
 * main - Opens an IPv4/TCP socket and listens to traffic on port 12345. Can
 *        accept an entering connection and waits for an incoming message.
 *        It prints the received message, then closes the connection
 *
 * Return: always zero
 */
int main(void)
{
	int client_id, sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addrport;
	struct sockaddr client_addr;
	socklen_t client_addr_size = sizeof(struct sockaddr);
	char *address, buffer[1024];

	if (sockid == -1)
		error_out("Socket");
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(PORT);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);


	if (bind(sockid, (struct sockaddr *)&addrport, sizeof(addrport)) == -1)
		close(sockid), error_out("Bind");

	if (listen(sockid, 1) == -1)
		close(sockid), error_out("Listen");

	printf("Server listening on port %d\n", PORT);
	client_id = accept(sockid, &client_addr, &client_addr_size);
	if (client_id == -1)
		close(sockid), error_out("Accept");

	address = inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr);
	printf("Client connected: %s\n", address);

	if (recv(client_id, buffer, sizeof(buffer), 0) == -1)
		close(sockid), close(client_id), error_out("recv");

	printf("Message received: \"%s\"\n", buffer);
	close(client_id);
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
