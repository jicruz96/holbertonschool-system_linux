#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "signals.h"

/* Our functions */
void gotcha(int);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    void (*handler)(int);
    int i;

    handler = current_handler_sigaction();
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    /* Set 'gotcha()` as the handler for SIGINT */
    handle_signal();

    handler = current_handler_sigaction();
    printf("Address of the 'gotcha' function: %#lx\n", (unsigned long int)&gotcha);
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    for (i = 0; ; i++)
    {
        printf("[%d] Wait for it ...\n", i);
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
