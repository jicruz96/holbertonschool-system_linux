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
    ls_config_t config = {DEFAULT_LS}; /* See config struct in header.h */
    dir_node_t *dirs_list = NULL;      /* See dir_node struct in header.h */

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

void print_list(file_node_t *file_list, ls_config_t config)
{
    char *delimiter = config.one_per_line ? "\n" : "  ";
    char *file_name;

    if (config.long_format)
    {
        /*         print_list_long(file_list, config);*/
        return;
    }

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
/*
void print_list_long(file_node_t *file_list, ls_config_t config)
{
    char permissions[11];
    char *file_name;

    for (; file_list != NULL; file_list = file_list->next)
    {
        get_permissions(permissions, file_list->mode);
        file_name = file_list->file_name;
        if (file_name[0] == '.')
        {
            if (config.show_hiddens)
            {
                printf("%s %2u %s %s %4lu %s %s\n", permissions,
                       (unsigned int)file_list->num_links, file_list->user,
                       file_list->group, (unsigned long)file_list->size,
                       file_list->mod_date, file_list->file_name);
            }
            else if (config.show_hiddens_alt)
            {
                if (file_name[1] != '.' && file_name[1] != '\0')
                    printf("%s %2u %s %s %4lu %s %s\n", permissions,
                           file_list->num_links, file_list->user,
                           file_list->group, (unsigned long)file_list->size,
                           file_list->mod_date, file_list->file_name);
            }
        }
        else
        {
            printf("%s %2u %s %s %4d %s %s\n", permissions,
                   file_list->num_links, file_list->user,
                   file_list->group, (unsigned long)file_list->size,
                   file_list->mod_date, file_list->file_name);
        }
    }
}

void get_permissions(char *buffer, mode_t mode)
{
    buffer[0] = get_type(mode);
    buffer[1] = mode & S_IRUSR ? 'r' : '-';
    buffer[2] = mode & S_IWUSR ? 'w' : '-';
    buffer[3] = mode & S_IXUSR ? 'x' : '-';
    buffer[4] = mode & S_IRGRP ? 'r' : '-';
    buffer[5] = mode & S_IWGRP ? 'w' : '-';
    buffer[6] = mode & S_IXGRP ? 'x' : '-';
    buffer[7] = mode & S_IROTH ? 'r' : '-';
    buffer[8] = mode & S_IWOTH ? 'w' : '-';
    buffer[9] = mode & S_IXOTH ? 'x' : '-';
    buffer[10] = '\0';
}

char get_type(mode_t mode)
{
    if (S_ISREG(mode))
        return ('-');
    if (S_ISDIR(mode))
        return ('d');
    if (S_ISCHR(mode))
        return ('c');
    if (S_ISBLK(mode))
        return ('b');
    if (S_ISFIFO(mode))
        return ('p');
    if (S_ISLNK(mode))
        return ('l');
    if (S_ISSOCK(mode))
        return ('s');
    return ('-');
}
*/
int make_lists(ls_config_t *config, char **av, int ac, int i, dir_node_t **h)
{
    int error_check, status = 0;

    if (i == ac)
        return ls(".", config, h);

    for (; i < ac; i++)
    {
        error_check = ls(av[i], config, h);
        if (error_check)
            status = error_check;
    }

    return (status);
}

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

file_node_t *read_dir(char *dir_name, DIR *dir_stream, ls_config_t *config)
{
    struct dirent *reader;
    file_node_t *new, *prev, *head = NULL;
    char file_path[512];
    struct stat file_info;
    unsigned int max_hard_links = 0;
    int max_size = 0, max_strlen = 0, len = 0;

    while ((reader = readdir(dir_stream)) != NULL)
    {
        new = malloc(sizeof(file_node_t));
        new->file_name = reader->d_name;
        new->next = NULL;
        new->prev = NULL;
        if (config->long_format)
        {
            sprintf(file_path, "%s/%s", dir_name, new->file_name);
            lstat(file_path, &file_info); /* Check for errors later pls */
            new->mode = file_info.st_mode;
            new->num_links = file_info.st_nlink;
            if (new->num_links > max_hard_links)
                max_hard_links = new->num_links;
            new->user = get_user(file_info.st_uid);
            len = _strlen(new->user);
            if (len > max_strlen)
                max_strlen = len;
            new->group = get_group(file_info.st_gid);
            len = _strlen(new->group);
            if (len > max_strlen)
                max_strlen = len;
            new->size = file_info.st_size;
            if (new->size > max_size)
                max_size = new->size;
            new->mod_date = ctime(&file_info.st_mtime);
        }
        if (head == NULL)
        {
            head = new;
        }
        else
        {
            prev->next = new;
            new->prev = prev;
        }
        prev = new;
    }
    config->max_hard_links = max_hard_links;
    config->max_size = max_size;
    config->max_strlen = max_strlen;
    return (head);
}
