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
	ssize_t bytes_read;
	char *bytes;

	/* Free linked list of readers when fd = -1 is passed */
	if (fd == -1)
	{
		for (; readers; readers = rd)
			rd = readers->next, free(readers->buf), free(readers);
		return (NULL);
	}
	/* Loop through known readers. If fd is found, find line */
	for (rd = readers; rd; rd = rd->next)
		if (rd->fd == fd)
		{
			if (rd->bytes <= 0)
				rd->bytes = read(fd, rd->buf, READ_SIZE);
			return (find_line(rd));
		}
	/* If not found, it's a new file. Create a new reader node */
	bytes = malloc(sizeof(char) * READ_SIZE);
	if (bytes == NULL)
		return (NULL);

	bytes_read = read(fd, bytes, READ_SIZE);
	if (bytes_read <= 0)
	{
		free(bytes);
		return (NULL);
	}

	rd = malloc(sizeof(reader_t));
	if (rd == NULL)
		return (NULL);
	rd->fd = fd, rd->buf = bytes, rd->bytes = bytes_read;
	rd->next = readers, readers = rd;

	return (find_line(rd));
}

/**
 * find_line - parses buffer, finds end of line, adjusts buffer, returns line
 * @rd: pointer to reader
 * Return: pointer to line (must be freed by user)
 **/
char *find_line(reader_t *rd)
{
	int i, j, line_size = 0, bytes_copied = 0;
	char *line = NULL, *tmp;

	while (rd->bytes > 0)
	{
		if (line_size < bytes_copied + rd->bytes + 1)
		{
			line_size += rd->bytes + 1;
			tmp = malloc(sizeof(char) * line_size);
			if (tmp == NULL)
			{
				free(line);
				return (NULL);
			}
			memcpy(tmp, line, bytes_copied);
			memset(tmp + bytes_copied, '\0', line_size - bytes_copied);
			free(line), line = tmp;
		}

		for (i = 0; i < rd->bytes; i++)
			if (rd->buf[i] == '\n')
			{
				rd->buf[i++] = '\0', rd->bytes -= i;
				for (j = 0; j < rd->bytes; j++)
				{
					if (j < i)
						(line + bytes_copied)[j] = rd->buf[j];
					if (i + j < READ_SIZE)
						rd->buf[j] = (rd->buf + i)[j], (rd->buf + i)[j] = '\0';
				}
				return (line);
			}

		memcpy(line + bytes_copied, rd->buf, rd->bytes);
		bytes_copied += rd->bytes;
		rd->bytes = read(rd->fd, rd->buf, READ_SIZE);
	}

	return (line);
}
