#include "sockets.h"
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>


/**
 * main - connects to a listening server. Takes 2 arguments:
 *        Usage: 2-client <host> <port>
 * @argc: argument count
 * @argv: argument array
 * Return: EXIT_SUCCESS on success | EXIT_FAILURE on failure
 */
int main(int argc, char *argv[])
{
	struct addrinfo hints = {0, AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0, 0};
	struct addrinfo *host_ai, *tmp;
	int status, client_id;


	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	status = getaddrinfo(argv[1], argv[2], &hints, &host_ai);
	if (status)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (EXIT_FAILURE);
	}

	client_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_id == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}

	for (tmp = host_ai; tmp; tmp = tmp->ai_next)
		if (connect(client_id, tmp->ai_addr, tmp->ai_addrlen) == 0)
		{
			printf("Connected to %s:%s\n", argv[1], argv[2]);
			freeaddrinfo(host_ai);
			close(client_id);
			return (EXIT_SUCCESS);
		}


	fprintf(stderr, "No valid address found for %s:%s\n", argv[1], argv[2]);
	freeaddrinfo(host_ai);
	close(client_id);
	return (EXIT_FAILURE);
}
