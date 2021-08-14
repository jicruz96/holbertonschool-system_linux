#include "sockets.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080
#define RES200 "HTTP/1.1 200 OK\r\n\r\n"

/**
 * make_response - responds to a request
 * @address: address to respond to
 * @request: adress's request
 * Return: response
 */
char *make_response(char *address, char *request)
{
	printf("Client connected: %s\n", address);
	printf("Raw request: \"%s\"\n", request);
	print_body_params(request);
	fflush(stdout);
	return (strdup(RES200));
}


/**
 * print_body_params - helper for take_requests()
 *
 * @request: request containing client request
 */
void print_body_params(char *request)
{
	char *body_params = strstr(request, "\r\n\r\n") + 4;
	char *path = strtok(strtok(strchr(request, ' ') + 1, " "), "?");
	char *key, *value;

	printf("Path: %s\n", path);

	for (
		key = strtok(body_params, "="), value = strtok(NULL, "&");
		key && value;
		key = strtok(NULL, "="), value = strtok(NULL, "&")
	)
		printf("Body param: \"%s\" -> \"%s\"\n", key, value);
}
