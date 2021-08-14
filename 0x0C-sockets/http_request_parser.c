#include "sockets.h"
#include <stdlib.h>


/**
 * get_header - get value of a header request field if it exists
 *
 * @headers: headers in a request
 * @field: desired field value
 * Return: value of field if it exists, NULL if it does not
 */
char *get_header(http_header_t *headers, char *field)
{
	for (; headers; headers = headers->next)
		if (strcmp(headers->field, field) == 0)
			return (headers->value);

	return (NULL);
}

/**
 * get_param - returns a parameter value from a list of parameters
 * @params: parameters
 * @key: key to search for
 * Return: value of key
 */
char *get_param(http_param_t *params, char *key)
{
	for (; params; params = params->next)
		if (strcmp(params->key, key) == 0)
			return (params->value);

	return (NULL);
}


/**
 * get_method - returns http_method_t (enum type) for the request method
 *
 * @method_str: the method (string) from the request
 * Return: http_method_t (enum type describing request type (see sockets.h))
 */
http_method_t get_method(char *method_str)
{
	char *method_strs[] = {
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"DELETE",
		"CONNECT",
		"OPTIONS",
		"TRACE"
	};
	size_t i, size = sizeof(method_strs) / sizeof(*method_strs);

	if (!method_str || !(*method_str))
		return (UNKNOWN);

	for (i = 0; i < size; i++)
		if (!strcmp(method_strs[i], method_str))
			return (i);

	return (UNKNOWN);
}

/**
 * http_headers_init - creates linked list of http_header_t structs. each node
 *                     in the list is equal to one request header field/value
 *                     pair
 *
 * @line: string of headers
 * Return: head of linked list of http_header_t structs
 **/
http_header_t *http_headers_init(char *line)
{
	http_header_t *tmp, *head = NULL, *prev = NULL;
	char *next_line;

	while (*line != '\r')
	{
		tmp = malloc(sizeof(http_header_t));
		if (!(*line) || !tmp)
		{
			free(tmp);
			while (head)
				tmp = head->next, free(head), head = tmp;
			return (NULL);
		}
		next_line  = strstr(line, "\r\n") + 2;
		tmp->field = strtok(line, ":");
		tmp->value = strtok(NULL, "\r\n") + 1;
		tmp->next  = NULL;
		if (prev)
			prev->next = tmp;
		else
			head = tmp;
		line = next_line;
		prev = tmp;
	}

	return (head);
}

/**
 * http_params_init - creates linked list of http_param_t structs. each node
 *                     in the list is equal to one key/value parameter pair
 *
 * @str: string of key value pairs, delimited by '&'s
 * Return: head of linked list of http_param_t structs
 **/
http_param_t *http_params_init(char *str)
{
	char *key, *value;
	http_param_t *head = NULL, *prev = NULL, *tmp;

	if (!str || !(*str))
		return (NULL);

	for (
		key = strtok(str, "="), value = strtok(NULL, "&");
		key && value;
		key = strtok(NULL, "="), value = strtok(NULL, "&")
	)
	{
		tmp = malloc(sizeof(http_header_t));
		if (!tmp)
		{
			while (head)
				tmp = head->next, free(head), head = tmp;
			return (NULL);
		}
		tmp->key   = key;
		tmp->value = value;
		tmp->next  = NULL;
		if (prev)
			prev->next = tmp;
		else
			head = tmp;
		prev = tmp;
	}

	return (head);

}

/**
 * http_request_init - constructs an instance of a http_request_t struct, which
 *                     describes an http request received by the server, by
 *                     parsing the raw HTTP request sent by the client.
 *
 *                     EXAMPLE:
 *                     * An HTTP Request looks like this:
 *                         <request-method> <request-uri> <http->version>\r\n
 *                         <request-header-field>: <header-value>\r\n
 *                             . . . (as many request headers as needed)
 *                         \r\n
 *                         <request-body>\0
 *
 *                     * Here's an example of an HTTP Request:
 *                         POST /todos HTTP/1.1\r\n
 *                         Content-Length: 29\r\n
 *                         \r\n
 *                         title=Hello&description=World
 *
 *                     * This function parses the request to create an
 *                     * http_request_t struct. Defined in `./sockets.h`
 *
 * @request_string: HTTP request received (as string)
 * Return: pointer to a struct descrbing the request
 */
http_request_t *http_request_init(char *request_string)
{
	http_request_t *request = malloc(sizeof(http_request_t));
	char *headers, *body, *query;

	if (!request)
		return (NULL);

	if (!(headers = strstr(request_string, "\r\n")) ||
		!(body = strstr(request_string, "\r\n\r\n")))
	{
		free(request);
		return (NULL);
	}
	headers += strlen("\r\n");
	body += strlen("\r\n\r\n");

	request->raw_request = strdup(request_string);
	request->method_str  = strtok(request_string, " ");
	request->method      = get_method(request->method_str);
	request->uri         = strtok(NULL, " ");
	request->version     = strtok(NULL, "\r\n");
	request->headers     = http_headers_init(headers);
	request->body        = *body ? strdup(body) : NULL;
	request->body_params = http_params_init(body);
	query = strchr(request->uri, '?'); /* ex: /todos?id=1 */
	if (query)	/* query -> ?id=1 */
		*(query++) = '\0'; /* nullify '?' and move query to next char */
	request->query_params = http_params_init(query); /* query -> id=1 */
	return (request);
}

/**
 * free_http_request - deallocates memory from a heap-alloacted http_request_t
 *                     struct
 *
 * @request: pointer to request
 */
void free_http_request(http_request_t *request)
{
	http_param_t *param_tmp;
	http_header_t *header_tmp;

	if (request)
	{
		free(request->raw_request);
		while (request->body_params)
		{
			param_tmp = request->body_params->next;
			free(request->body_params);
			request->body_params = param_tmp;
		}
		while (request->headers)
		{
			header_tmp = request->headers->next;
			free(request->headers);
			request->headers = header_tmp;
		}
		free(request);
	}
}
