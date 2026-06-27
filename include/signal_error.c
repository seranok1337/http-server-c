#include <stdlib.h>
#include <stdio.h>

void signal_error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}