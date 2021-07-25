#include <hreadelf.h>

/**
 * read_attr - reads an ELF file attribute from an ELF file
 * @fd: ELF file descriptor
 * @attr: pointer to attribute where value will be stored
 * @size: size of attribute
 * @encoding: encoding of attribute
 * Return: 1 on success | 0 on failure
 **/
int read_attr(int fd, void *attr, size_t size, uint8_t encoding)
{
	if (read(fd, attr, size) != (ssize_t)size)
		return (0);

	if (encoding == ELFDATA2MSB)
	{
		if (size == sizeof(uint16_t))
			*((uint16_t *)attr) = __bswap_16(*((uint16_t *)attr));
		else if (size == sizeof(uint32_t))
			*((uint32_t *)attr) = __bswap_32(*((uint32_t *)attr));
		else if (size == sizeof(uint64_t))
			*((uint64_t *)attr) = __bswap_64(*((uint64_t *)attr));
	}

	return (1);
}
