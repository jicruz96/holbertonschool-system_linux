#include "sockets.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

/**
 * main - Opens an IPv4/TCP socket and listens to traffic on port 12345.
 *        This is a learning tool only. The program can accept an entering
 *        connection, and nothing else.
 *
 * Return: always zero
 */
int main(void)
{
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addrport;
	struct sockaddr client_addr;
	socklen_t client_addr_size = sizeof(struct sockaddr);
	char *address;

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(PORT);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);


	if (bind(sockid, (struct sockaddr *)&addrport, sizeof(addrport)) == -1)
	{
		perror("Bind:");
		return (EXIT_FAILURE);
	}

	if (listen(sockid, 1) == -1)
	{
		perror("Listen:");
		return (EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);
	if (accept(sockid, &client_addr, &client_addr_size) == -1)
	{
		perror("Accepting error:");
		return (EXIT_FAILURE);
	}

	address = inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr);
	printf("Client connected: %s\n", address);

	close(sockid);
	return (0);
}
