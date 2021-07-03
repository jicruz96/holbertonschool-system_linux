#include "sockets.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

#define PORT 12345

/**
 * main - Opens an IPv4/TCP socket and listens to traffic on port 12345.
 *        This is just a learning tool, as this server does not accept any
 *        entering connections.
 *
 * Return: EXIT_SUCESS on sucess | EXIT_FAILURE if socket binding fails
 */
int main(void)
{
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addrport;


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

	while (1)
		continue;

	close(sockid);
	return (0);
}
