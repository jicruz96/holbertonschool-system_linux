#include "signals.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * gotcha - prints "Gotcha! [<signum]" when SIGINT sent to process
 * @signum: signal number
 **/
void gotcha(int signum)
{
	char signum_str[3] = {'\0', '\0', '\0'};
	char *gotcha_message = "Caught ";

	if (signum > 9)
	{
		signum_str[0] = (signum / 10) + '0';
		signum_str[1] = (signum % 10) + '0';
	}
	else
	{
		signum_str[0] = signum + '0';
	}

	write(STDOUT_FILENO, gotcha_message, strlen(gotcha_message));
	write(STDOUT_FILENO, signum_str, strlen(signum_str));
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * handle_signal - handles the SIGINT signal
 * Return: 0 on success, -1 on error
 **/
int handle_signal(void)
{
	if (signal(SIGINT, gotcha) == SIG_ERR)
		return (FAILURE);
	return (SUCCESS);
}


/**
 * main - entry point
 * Return: EXIT_SUCCESS on success | EXIT_FAILURE on failure
 **/
int main(void)
{
	char *exit_message = "Signal received\n";

	handle_signal();
	pause();
	write(STDOUT_FILENO, exit_message, strlen(exit_message));
	return (EXIT_SUCCESS);
}
