#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define PORT         12345

/**
 * main - Opens an IPv4/TCP socket and listens to traffic on port 12345.
 *        This is just a learning tool, as this server does not accept any
 *        entering connections.
 *
 * Return: EXIT_SUCESS on sucess | EXIT_FAILURE if socket binding fails
 */
int main(void)
{
	int server_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in port;


	port.sin_family = AF_INET;
	port.sin_port = htons(PORT);
	port.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_id, (struct sockaddr *)&port, sizeof(port)) == -1)
	{
		perror("Bind");
		return (EXIT_FAILURE);
	}

	if (listen(server_id, 1) == -1)
	{
		perror("Listen");
		return (EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	while (1)
		continue;

	close(server_id);
	return (EXIT_SUCCESS);
}
