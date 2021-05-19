#include "multithreading.h"
#include <stdlib.h>

/**
 * prime_factors - factors a number into a list of prime factors
 * @s: string representation of the number to factor
 * Return: list_t of prime factors
 **/
list_t *prime_factors(char const *s)
{
	unsigned long n = strtoul(s, NULL, 10);
	unsigned long *tmp, p = 2;
	list_t *list = malloc(sizeof(list_t));

	list_init(list);
	while (p * p <= n)
	{
		while (n % p == 0)
		{
			tmp = malloc(sizeof(unsigned long));
			*tmp = p;
			list_add(list, (void *)tmp);
			n /= p;
		}

		p += 1 + (p != 2);
	}

	if (n >= 2)
	{
		tmp = malloc(sizeof(unsigned long));
		*tmp = n;
		list_add(list, (void *)tmp);
	}
	return (list);
}
