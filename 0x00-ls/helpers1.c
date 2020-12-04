#include "header.h"

/**
 * print_list - prints lists
 * @file_list: list of files to print
 * @config: config
 **/
void print_list(file_node_t *file_list, ls_config_t config)
{
	char *delimiter = config.one_per_line ? "\n" : "  ";
	char *file_name;

	for (; file_list != NULL; file_list = file_list->next)
	{
		file_name = file_list->file_name;
		if (file_name[0] == '.')
		{
			if (config.show_hiddens)
			{
				printf("%s%s", file_name, delimiter);
			}
			else if (config.show_hiddens_alt)
			{
				if (file_name[1] != '.' && file_name[1] != '\0')
					printf("%s%s", file_name, delimiter);
			}
		}
		else
		{
			printf("%s%s", file_name, delimiter);
		}
	}
	if (delimiter[0] != '\n')
		putchar('\n');
}

/**
 * make_lists - makes lists of all directories in the arguments array
 * @config: ls config
 * @av: argument vector
 * @ac: argument count
 * @i: index where arguments start in argument vector
 * @h: head of linked list of directories to be listed
 * Return: status
 **/
int make_lists(ls_config_t *config, char **av, int ac, int i, dir_node_t **h)
{
	int error_check, status = 0;

	if (i == ac)
		return (ls(".", config, h));

	for (; i < ac; i++)
	{
		error_check = ls(av[i], config, h);
		if (error_check)
			status = error_check;
	}

	return (status);
}

/**
 * ls - ls
 * @dir_name: dir name
 * @config: config
 * @head: head of list of dirs to be ls'd
 * Return: status
 **/
int ls(char *dir_name, ls_config_t *config, dir_node_t **head)
{
	dir_node_t *new = malloc(sizeof(dir_node_t));
	DIR *dir_stream = opendir(dir_name);
	dir_node_t *tmp;

	new->dir_name = dir_name;
	new->error_code = 0;
	new->list = NULL;
	new->next = NULL;
	new->prev = NULL;

	if (dir_stream == NULL)
		new->error_code = errno;
	else
		new->list = read_dir(dir_name, dir_stream, config);

	if (*head == NULL)
	{
		*head = new;
	}
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}

	return (new->error_code);
}

/**
 * read_dir - reads dir
 * @dir_name: dir name
 * @dir_stream: dir stream
 * @config: config
 * Return: linked list of files with file info
 **/
file_node_t *read_dir(char *dir_name, DIR *dir_stream, ls_config_t *config)
{
	struct dirent *reader;
	file_node_t *new, *prev, *head = NULL;
	char file_path[512];
	struct stat file_info;
	unsigned int max_hard_links = 0;
	int max_size = 0, max_strlen = 0, len = 0;

	for (; (reader = readdir(dir_stream)) != NULL; prev = new)
	{
		new = malloc(sizeof(file_node_t));
		new->file_name = reader->d_name;
		new->next = NULL;
		new->prev = NULL;
		if (config->long_format)
		{
			sprintf(file_path, "%s/%s", dir_name, new->file_name);
			lstat(file_path, &file_info); /* Check for errors later pls */
			get_long_stats(new, file_info);
			new->num_links = file_info.st_nlink;
			if (new->num_links > max_hard_links)
				max_hard_links = new->num_links;
			len = _strlen(new->user);
			if (len > max_strlen)
				max_strlen = len;
			len = _strlen(new->group);
			if (len > max_strlen)
				max_strlen = len;
			new->size = file_info.st_size;
			if (new->size > max_size)
				max_size = new->size;
		}
		if (head == NULL)
			head = new;
		else
			prev->next = new, new->prev = prev;
	}
	config->max_hard_links = max_hard_links;
	config->max_size = max_size;
	config->max_strlen = max_strlen;
	return (head);
}

/**
 * get_long_stats - helper function to read_dir
 * @new: new node
 * @file_info: info
 **/
void get_long_stats(file_node_t *new, struct stat file_info)
{
	new->mode = file_info.st_mode;
	new->user = get_user(file_info.st_uid);
	new->group = get_group(file_info.st_gid);
	new->mod_date = ctime(&file_info.st_mtime);
}
