#include "header.h"

/**
 * main - custom ls command
 * @argc: argument count
 * @argv: argument array
 * Return: exit status | 0 on success | 2 on failure
 **/
int main(int argc, char **argv)
{
	int i, status = 0, check = 0, dirs = 0;
	dir_node_t *dirs_list = NULL;
	file_node_t *file_list = NULL;
	DIR *dir_stream;
	ls_config_t flags = {&print_list, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			if (set_flags(argv[i], &flags))
			{
				free_everything(dirs_list, file_list);
				return (2);
			}
		}
		else
		{
			dir_stream = opendir(argv[i]);
			if (dir_stream == NULL && (errno == ENOTDIR || errno == ENOENT))
				check = add_file_node(argv[i], "", &file_list);
			else
				check = add_dir_node(argv[i], dir_stream, &dirs_list), dirs++;
		}
		status = check ? check : status;
	}
	if (file_list || dirs > 1 || (file_list == NULL && status != 0))
		flags.print_dir_name = true;
	if (dirs == 0 && file_list == NULL && status == 0)
		add_dir_node(".", opendir("."), &dirs_list);
	flags.printer(file_list, &flags);
	if (dirs && file_list)
		putchar('\n');
	check = print_dirs(dirs_list, &flags, flags.printer);
	status = check ? check : status;
	free_everything(dirs_list, file_list);
	return (status ? 2 : 0);
}

/**
 * set_flags - sets configurations for ls command
 * @arg: argument
 * @flags: flags struct
 * Return: 0 on success | 2 if invalid option encountered
 **/
int set_flags(char *arg, ls_config_t *flags)
{
	int i;

	for (i = 1; arg[i] != '\0'; i++)
		if (arg[i] == '1')
			flags->one_per_line = true;
		else if (arg[i] == 'a')
			flags->dot = true;
		else if (arg[i] == 'A')
			flags->dot_alt = true;
		else if (arg[i] == 'l')
			flags->printer = &print_list_long;
		else if (arg[i] == 'r')
			flags->reversed = true;
		else if (arg[i] == 'R')
			flags->recursive = true;
		else if (arg[i] == 't')
			flags->sort_by_time = true;
		else if (arg[i] == 'S')
			flags->sort_by_size = true;
		else
		{
			fprintf(stderr, "hls: invalid option -- '%c'\n", arg[i]);
			fprintf(stderr, "Try 'hls --help' for more information.\n");
			return (2);
		}
	return (0);
}

/**
 * add_dir_node - add a directory node to a dir_node_t linked list
 * @dir_name: directory name
 * @stream: directory stream
 * @head: head of dir_node_t list where new node will be added
 * Return: 0 on success | an error code on failure
 **/
int add_dir_node(char *dir_name, DIR *stream, dir_node_t **head)
{
	dir_node_t *new_node, *tmp;
	struct dirent *read;
	file_node_t *file_head = NULL;
	int error_code = errno;

	new_node = malloc(sizeof(dir_node_t));
	new_node->dir_name = dir_name;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->error_code = stream ? 0 : error_code;
	if (new_node->error_code == 0)
	{
		while ((read = readdir(stream)) != NULL)
			error_code = add_file_node(read->d_name, dir_name, &file_head);
		closedir(stream);
	}
	new_node->list = file_head;
	if (*head == NULL)
	{
		*head = new_node;
		return (new_node->error_code);
	}
	tmp = *head;
	while (tmp && which_goes_first(dir_name, tmp->dir_name) != dir_name)
		new_node->prev = tmp, tmp = tmp->next;
	if (tmp)
	{
		new_node->prev = tmp->prev;
		if (tmp->prev)
			tmp->prev->next = new_node;
		new_node->next = tmp;
		tmp->prev = new_node;
		if (tmp == *head)
			*head = new_node;
	}
	else
	{
		new_node->prev->next = new_node;
	}
	return (new_node->error_code);
}

/**
 * add_file_node - add a file node to a file_node_t linked list
 * @file_name: file name
 * @dir_name: dir name
 * @head: head of file_node_t list where new node will be added
 * Return: 0 on success | an error code if lstat fails
 **/
int add_file_node(char *file_name, char *dir_name, file_node_t **head)
{
	file_node_t *new_node, *tmp;
	struct stat *info = malloc(sizeof(struct stat));
	char file_path[512];
	int i, j;

	for (i = 0; dir_name[i]; i++)
		file_path[i] = dir_name[i];
	if (i)
		file_path[i++] = '/';
	for (j = 0; file_name[j]; i++, j++)
		file_path[i] = file_name[j];
	file_path[i] = '\0';
	if (lstat(file_path, info) == -1)
	{
		free(info);
		return (print_error_message(file_name));
	}
	new_node = file_node_init(file_name, dir_name, info);
	if (*head == NULL)
	{
		*head = new_node;
		return (0);
	}
	tmp = *head;
	while (tmp && which_goes_first(file_name, tmp->name) != file_name)
		new_node->prev = tmp, tmp = tmp->next;
	if (tmp)
	{
		new_node->prev = tmp->prev;
		if (tmp->prev)
			tmp->prev->next = new_node;
		new_node->next = tmp;
		tmp->prev = new_node;
		if (tmp == *head)
			*head = new_node;
	}
	else
		new_node->prev->next = new_node;
	return (0);
}

/**
 * file_node_init - init file node
 * @name: name
 * @dir_name: dir name
 * @info: info
 * Return: pointer to node
 **/
file_node_t *file_node_init(char *name, char *dir_name, struct stat *info)
{
	file_node_t *new_node;

	new_node = malloc(sizeof(file_node_t));
	new_node->name = duplicate_string(name);
	new_node->dir_name = duplicate_string(dir_name);
	new_node->info = info;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}
