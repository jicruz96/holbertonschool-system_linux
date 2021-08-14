#include "sockets.h"
#include <string.h>
#include <stdlib.h>

/**
 * make_repr - makes repr
 *
 * @id: id
 * @title: title
 * @description: description
 * Return: repr
 */
char *make_repr(int id, char *title, char *description)
{
#define DICT     "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}"

	size_t len = strlen(title) + strlen(description) + strlen(DICT) + 10;
	char *repr = malloc(sizeof(char) * len);

	if (repr)
		sprintf(repr, DICT, id, title, description);
	return (repr);
}


/**
 * add_todo - adds todo item
 *
 * @todos: todos
 * @id: id
 * @title: title
 * @description: description
 */
void add_todo(todo_t *todos, int id, char *title, char *description)
{
	todo_t *todo = todos + id;

	todo->id = id;
	todo->title = strdup(title);
	todo->description = strdup(description);
	todo->repr = make_repr(id, title, description);
	todo->repr_len = strlen(todo->repr);
}
