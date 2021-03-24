/**
 * bswap_string - MSB to LSB encoding for a string
 * @str: string
 * @size: size of string
 **/
void bswap_string(char *str, int size)
{
	int i;
	char tmp;

	for (i = 0; i * 2 < size; i++)
	{
		tmp = str[i];
		str[i] = str[size - 1 - i];
		str[size - 1 - i] = tmp;
	}

}
