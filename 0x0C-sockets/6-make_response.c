#include <stdio.h>
#include <string.h>

/**
 * print_headers - helper for take_requests()
 * @buffer: buffer containing client request
 */
static void print_headers(char *buffer)
{
	char *headers = strchr(buffer, '\n') + 1;
	char *header  = strtok(headers, ":");
	char *value   = strtok(NULL, "\r");

	while (header && value)
	{
		printf("Header: \"%s\" -> \"%s\"\n", header + (*header == '\n'), ++value);
		header = strtok(NULL, ":");
		value = strtok(NULL, "\r");
	}

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
	print_headers(request);
	fflush(stdout);
	return (strdup("HTTP/1.1 200 OK\r\n\r\n"));
}
