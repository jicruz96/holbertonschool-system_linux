#include "header.h"

/**
 * main - custom ls command
 * @argc: argument count
 * @argv: argument array
 * Return: exit status | 0 on success | 2 on failure
 **/
int main(int argc, char **argv)
{
	int i, error_check = 0;
	ls_config_t config = {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	dir_node_t *dirs_list = NULL, *tmp;

	/* Initialize config struct by including the program name in config */
	config.program_name = argv[0];

	/* Parse through arguments to get configuration. */
	i = get_config(argc, argv, &config);

	/* make_lists stores the final list of lists in dirs_list */
	error_check = make_lists(&config, argv, argc, i, &dirs_list);
	/* If there is only one directory, print its list of contents */
	if (dirs_list->next == NULL)
	{
		if (dirs_list->error_code)
			print_error_message(dirs_list, argv[0]);
		else
			print_list(dirs_list->list, config);
	}
	else /* else, print the directory name before and a newline after */
	{
		for (tmp = dirs_list; tmp != NULL; tmp = tmp->next)
		{
			if (tmp->error_code)
			{
				print_error_message(tmp, argv[0]);
			}
			else
			{
				printf("%s:\n", tmp->dir_name);
				print_list(tmp->list, config);
			}

			if (tmp->next != NULL)
				putchar('\n');
		}
	}
	free_everything(dirs_list);
	return (error_check);
}

/**
 * free_everything - yup
 * @head: head
 **/
void free_everything(dir_node_t *head)
{
	file_node_t *f, *f_prev;
	dir_node_t *prev;

	while (head)
	{
		f = head->list;

		while (f)
		{
			f_prev = f;
			f = f->next;
			free(f_prev);
		}
		prev = head;
		head = head->next;
		free(prev);
	}
}
