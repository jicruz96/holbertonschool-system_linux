#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

/**
 * hints_init - defines the hints addrinfo struct, which we use for getaddrinfo
 *
 * @hints: pointer to addrinfo struct
 */
static void hints_init(struct addrinfo *hints)
{
	if (hints)
	{
		hints->ai_flags     = 0;
		hints->ai_family    = AF_INET;
		hints->ai_socktype  = SOCK_STREAM;
		hints->ai_protocol  = IPPROTO_TCP;
		hints->ai_addrlen   = 0;
		hints->ai_addr      = NULL;
		hints->ai_canonname = NULL;
		hints->ai_next      = NULL;
	}
}

/**
 * main - connects to a listening server. Takes 2 arguments:
 *        Usage: 2-client <host> <port>
 * @argc: argument count
 * @argv: argument array
 * Return: EXIT_SUCCESS on success | EXIT_FAILURE on failure
 */
int main(int argc, char *argv[])
{
	struct addrinfo hints, *host_addrinfo, *tmp;
	int status, client_id;


	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	hints_init(&hints);
	status = getaddrinfo(argv[1], argv[2], &hints, &host_addrinfo);
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

	for (tmp = host_addrinfo; tmp; tmp = tmp->ai_next)
	{
		if (connect(client_id, tmp->ai_addr, tmp->ai_addrlen) == 0)
		{
			printf("Connected to %s:%s\n", argv[1], argv[2]);
			freeaddrinfo(host_addrinfo);
			close(client_id);
			return (EXIT_SUCCESS);
		}
	}

	fprintf(stderr, "No valid address found for %s:%s\n", argv[1], argv[2]);
	freeaddrinfo(host_addrinfo);
	close(client_id);
	return (EXIT_FAILURE);
}
