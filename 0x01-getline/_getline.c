#include "_getline.h"

/**
 * _getline -   returns string leading up to newline in a file. subsequent
 *              calls return the following line, and so on.
 * @fd:	file descriptor
 * Return: string (not including newline, null-terminated)
 **/
char *_getline(const int fd)
{
	static reader_t *readers;
	reader_t *rd;

	if (fd == -1)
		return (free_readers(&readers));

	for (rd = readers; rd; rd = rd->next)
		if (rd->fd == fd)
		{
			if (rd->bytes <= 0)
				rd->bytes = read(fd, rd->buf, READ_SIZE);
			return (return_line(rd));
		}

	rd = reader_init(fd);
	if (rd == NULL)
		return (NULL);
	rd->next = readers;
	readers = rd;
	return (return_line(rd));
}

/**
 * return_line - parses buffer, finds end of line, adjusts buffer, returns line
 * @rd: pointer to reader
 * Return: pointer to line (must be freed by user)
 **/
char *return_line(reader_t *rd)
{
	int i, j, line_size = 0, bytes_copied = 0;
	char *line = NULL, *tmp, *next, *end_of_line;

	for (; rd->bytes > 0; rd->bytes = read(rd->fd, rd->buf, READ_SIZE))
	{
		if (line_size < bytes_copied + rd->bytes + 1)
		{
			line_size += rd->bytes + 1, tmp = malloc(sizeof(char) * line_size);
			if (tmp == NULL)
			{
				free(line);
				return (NULL);
			}
			_memcpy(tmp, line, bytes_copied);
			memset(tmp + bytes_copied, '\0', line_size - bytes_copied);
			free(line), line = tmp;
		}
		for (i = 0; i < rd->bytes; i++)
			if (rd->buf[i] == '\n')
			{
				rd->buf[i++] = '\0', rd->bytes -= i;
				next = rd->buf + i, end_of_line = line + bytes_copied;
				for (j = 0; j < i; j++, bytes_copied++)
				{
					end_of_line[j] = rd->buf[j];
					if (i + j < READ_SIZE)
						rd->buf[j] = next[j], next[j] = '\0';
				}
				while (j < rd->bytes && (i + j) < READ_SIZE)
				{
					rd->buf[j] = next[j];
					next[j] = '\0';
					j += 1;
				}
				return (line);
			}
		_memcpy(line + bytes_copied, rd->buf, rd->bytes);
		bytes_copied += rd->bytes;
	}
	return (line);
}

/**
 * reader_init - initializes new "reader" node. The reader associates a file
 *				 descriptor to a buffer. _getline uses this buffer to parse
 *				 through the lines of the file and read more file contents
 *				 when necessary.
 *
 * @fd: file descriptor
 * Return: pointer to new node
 **/
reader_t *reader_init(int fd)
{
	reader_t *new;
	ssize_t bytes_read;
	char *bytes = malloc(sizeof(char) * READ_SIZE);

	bytes_read = read(fd, bytes, READ_SIZE);
	if (bytes_read <= 0)
	{
		free(bytes);
		return (NULL);
	}
	new = malloc(sizeof(reader_t));
	new->fd = fd;
	new->buf = bytes;
	new->bytes = bytes_read;
	new->i = 0;
	new->next = NULL;
	new->buf_size = READ_SIZE;
	return (new);
}

/**
 * free_readers - frees all reader nodes and node contents
 * @readers: pointer to head of linked list of readers
 * Return: NULL, always, which _getline immediately returns
 **/
char *free_readers(reader_t **readers)
{
	reader_t *curr, *tmp;

	if (!readers || !(*readers))
		return (NULL);

	for (curr = *readers; curr; curr = tmp)
	{
		tmp = curr->next;
		free(curr->buf);
		free(curr);
	}
	*readers = NULL;
	return (NULL);
}

/**
 * _memcpy - custom memcpy. Normal memcpy raises an error if the two
 *			 pointer areas overlap. This one does not.
 *
 * @dest: pointer to buffer where memory will be copies
 * @src: source of content to be copied
 * @size: number of bytes to copy over
 * Return: pointer to dest
 **/
char *_memcpy(char *dest, char *src, size_t size)
{
	unsigned int i;

	for (i = 0; i < size; i++)
		dest[i] = src[i];

	return (dest);
}
