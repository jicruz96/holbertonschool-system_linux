#include <stdio.h>
#include <string.h>

/**
 * make_response - creates a response to a request
 *
 * @address: client adress to respond to (only needed for printing purposes)
 * @request: buffer containing request from address
 * Return: response string
 */
char *make_response(char *address, char *request)
{
	printf("Client connected: %s\n", address);
	printf("Raw request: \"%s\"\n", request);
	printf("Method: %s\n", strtok(request, " "));
	printf("Path: %s\n", strtok(NULL, " "));
	printf("Version: %s\n", strtok(NULL, "\r\n"));
	return (strdup("HTTP/1.1 200 OK\r\n\r\n"));
}
