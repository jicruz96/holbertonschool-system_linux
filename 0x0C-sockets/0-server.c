#include "sockets.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

#define PORT 12345
#define SOCKADDR_IN_INIT {sizeof(struct sockaddr_in), AF_INET, \
							htons(PORT), {htonl(INADDR_ANY)}, {0}}
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
	struct sockaddr_in addrport = SOCKADDR_IN_INIT;

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
