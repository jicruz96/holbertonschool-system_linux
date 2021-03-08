#include "signals.h"

/**
 * current_handler_sigaction - retrieves the current handler of SIGINT
 * Return: pointer to signal handler function
 */
void (*current_handler_sigaction(void))(int)
{
	struct sigaction current_sa;

	if (sigaction(SIGINT, NULL, &current_sa) == -1)
		return (NULL);
	return (current_sa.sa_handler);
}
