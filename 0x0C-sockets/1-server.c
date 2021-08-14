#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define PORT         12345

/**
 * main - Opens an IPv4/TCP socket and listens to traffic on port 12345.
 *        This is a learning tool only. The program can accept an entering
 *        connection, and nothing else.
 *
 * Return: always zero
 */
int main(void)
{
	int server_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	socklen_t addr_size = sizeof(struct sockaddr);
	struct sockaddr_in server_addr, client_addr;

	if (server_id == -1)
	{
		perror("Socket:");
		return (EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_id, (struct sockaddr *)&server_addr, addr_size) == -1)
	{
		perror("Bind:");
		return (EXIT_FAILURE);
	}

	if (listen(server_id, 1) == -1)
	{
		perror("Listen:");
		return (EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	if (accept(server_id, (struct sockaddr *)&client_addr, &addr_size) == -1)
	{
		perror("Accepting error:");
		return (EXIT_FAILURE);
	}

	printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

	close(server_id);
	return (0);
}
