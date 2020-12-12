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
			return (return_line(rd));

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
	int i, len = rd->num_bytes - rd->i;
	char *line;

	if (len < 0)
		return (NULL);
	for (i = rd->i; i < rd->num_bytes || rd->num_bytes != rd->buf_size; i++)
		if (rd->num_bytes == i || rd->buf[i] == '\n')
		{
			if (rd->num_bytes == i && !rd->buf[i])
				return (NULL);
			if (rd->num_bytes != i)
				rd->buf[i] = '\0';
			line = malloc(sizeof(char) * (i - rd->i + 1));
			_memcpy(line, rd->buf + rd->i, i - rd->i + 1);
			rd->i = i + 1;
			return (line);
		}
	if (rd->i)
		_memcpy(rd->buf, rd->buf + rd->i, len);
	if (READ_SIZE + len > rd->buf_size)
	{
		line = malloc(READ_SIZE + len);
		if (line == NULL)
			return (NULL);
		_memcpy(line, rd->buf, rd->buf_size);
		free(rd->buf), rd->buf = line;
	}
	memset(rd->buf + len, '\0', rd->num_bytes - len);
	rd->buf_size = READ_SIZE + len;
	rd->i = 0;
	rd->num_bytes = read(rd->fd, rd->buf + len, READ_SIZE);
	if (rd->num_bytes == -1)
	{
		rd->num_bytes = len;
		return (NULL);
	}
	rd->num_bytes += len;
	return (return_line(rd));
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
	char *bytes = malloc(sizeof(char) * READ_SIZE);
	ssize_t res;

	memset(bytes, '\0', READ_SIZE);
	res = read(fd, bytes, READ_SIZE);
	if (res <= 0)
	{
		free(bytes);
		return (NULL);
	}
	new = malloc(sizeof(reader_t));
	new->fd = fd;
	new->buf = bytes;
	new->num_bytes = res;
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
