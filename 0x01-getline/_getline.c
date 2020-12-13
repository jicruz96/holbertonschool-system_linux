
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
	char *bytes;
	int bytes_read;

	if (fd == -1)
	{
		for (rd = readers; rd; rd = readers)
		{
			readers = rd->next;
			free(rd->buf);
			free(rd);
		}
		return (NULL);
	}

	for (rd = readers; rd; rd = rd->next)
		if (rd->fd == fd)
		{
			if (rd->bytes <= 0)
				rd->bytes = read(fd, rd->buf, READ_SIZE);
			return (find_line(rd));
		}

	bytes = malloc(sizeof(char) * READ_SIZE);
	bytes_read = read(fd, bytes, READ_SIZE);
	if (bytes_read <= 0)
	{
		free(bytes);
		return (NULL);
	}
	rd = malloc(sizeof(reader_t));
	if (rd == NULL)
		return (NULL);
	rd->fd = fd;
	rd->buf = bytes;
	rd->bytes = bytes_read;
	rd->next = readers;
	readers = rd;
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
			line_size += rd->bytes + 1, tmp = malloc(sizeof(char) * line_size);
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
				for (j = 0; j < i; j++)
				{
					(line + bytes_copied)[j] = rd->buf[j];
					if (i + j < READ_SIZE)
						rd->buf[j] = (rd->buf + i)[j], (rd->buf + i)[j] = '\0';
				}
				for (; j < rd->bytes && (i + j) < READ_SIZE; j++)
					rd->buf[j] = (rd->buf + i)[j], (rd->buf + i)[j] = '\0';
				return (line);
			}

		memcpy(line + bytes_copied, rd->buf, rd->bytes);
		bytes_copied += rd->bytes;
		rd->bytes = read(rd->fd, rd->buf, READ_SIZE);
	}

	return (line);
}

/**
 * return_line - merges two buffers, erases unneeded memory, returns buffer
 * @line: line buffer
 * @reader: reader node with buffer with extra chars and remaining byte count
 * @buf_len: size of reader's buffer stuff
 * @offset: what index of line to start copying extra into
 * Return: line
 **/
char *return_line(char *line, reader_t *reader, int buf_len, int offset)
{
	int i;
	char *next = reader->buf + buf_len;
	char *line_end = line + offset;

	for (i = 0; i < buf_len; i++)
	{
		line_end[i] = reader->buf[i];
		if (i + buf_len < READ_SIZE)
		{
			reader->buf[i] = next[i];
			next[i] = '\0';
		}
	}
	for (; i < reader->bytes && (i + buf_len) < READ_SIZE; i++)
	{
		reader->buf[i] = next[i];
		next[i] = '\0';
	}

	return (line);
}
