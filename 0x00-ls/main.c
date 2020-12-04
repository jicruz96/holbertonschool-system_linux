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
	dir_node_t *dirs_list = NULL; /* See dir_node struct in header.h */

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
		for (; dirs_list != NULL; dirs_list = dirs_list->next)
		{
			if (dirs_list->error_code)
			{
				print_error_message(dirs_list, argv[0]);
			}
			else
			{
				printf("%s:\n", dirs_list->dir_name);
				print_list(dirs_list->list, config);
			}

			if (dirs_list->next != NULL)
				putchar('\n');
		}
	}
	return (error_check);
}
