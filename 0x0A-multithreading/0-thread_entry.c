#include "multithreading.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>

/**
 * thread_entry - entry point to a new thread
 * @arg: address of a string to be printed, followed by a new line
 * Return: ???
 **/
void *thread_entry(void *arg)
{
	if (arg)
		write(STDOUT_FILENO, arg, strlen((char *)arg));
	write(STDOUT_FILENO, "\n", 1);
	pthread_exit(NULL);
	return (NULL);
}
