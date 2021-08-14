#include <stdio.h>
#include <string.h>


/**
 * print_path_and_queries - helper for take_requests()
 *
 * @buffer: buffer containing client request
 */
static void print_path_and_queries(char *buffer)
{
	char *path, *key, *value;

	path = strtok(strtok(strchr(buffer, ' ') + 1, " "), "?");

	printf("Path: %s\n", path);

	for (
		key = strtok(NULL, "="), value = strtok(NULL, "&/");
		key && value;
		key = strtok(NULL, "="), value = strtok(NULL, "&/")
	)
		printf("Query: \"%s\" -> \"%s\"\n", key, value);
}

/**
 * make_response - creates response to a request
 *
 * @address: adress to respond to (only needed for info printing purposes)
 * @request: buffer containing request from address
 * Return: response
 */
char *make_response(char *address, char *request)
{
	printf("Client connected: %s\n", address);
	printf("Raw request: \"%s\"\n", request);
	print_path_and_queries(request);
	fflush(stdout);
	return (strdup("HTTP/1.1 200 OK\r\n\r\n"));
}
