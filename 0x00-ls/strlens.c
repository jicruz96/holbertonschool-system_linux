#include "header.h"

/**
 * _strlen - custom strlen
 * @str: string
 * Return: string length
 **/
int _strlen(char *str)
{
	int i = 0;

	if (str)
	{
		while (str[i] != '\0')
			i++;
	}
	return (i);
}
