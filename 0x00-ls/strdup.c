#include "header.h"

/**
* _strdup - returns a pointer to a space in memory that has a copy of a string
* @str: string to be copied to newly allocated memory space
* Return: char pointer to string, or NULL if memory allocation fails
*/
char *_strdup(char *str)
{
	int i, len;
	char *ptr;

	if (str == NULL)
		return (NULL);
	for (len = 0; str[len]; len++) /* get string length */
		;
	len++;
	ptr = (char *)malloc(len); /* allocate memory */
	if (ptr == NULL)		   /* check for NULL */
		return (NULL);
	for (i = 0; i < len; i++) /* copy string */
		ptr[i] = str[i];
	return (ptr); /* return pointer */
}
