#include "signals.h"

/**
 * current_handler_signal - retrieves the current handler of SIGINT
 * Return: pointer to signal handler function
 */
void (*current_handler_signal(void))(int)
{
	sig_t handler;

	handler = signal(SIGINT, SIG_IGN);
	if (handler == SIG_ERR || signal(SIGINT, handler) == SIG_ERR)
		return (NULL);
	return (handler);
}
