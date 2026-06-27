#include <stdio.h>
#include <stdlib.h>
#include "./signal_error.h"

FILE *init_file(char *path)
{
    FILE *fptr = fopen(path, "r");

    if (fptr == NULL)
    {
        signal_error("failed to open file");
        return NULL;
    }

    return fptr;
}

char *read_line(FILE *fptr)
{
    char *line = malloc(1000);
    
    fgets(line, sizeof(line), fptr);

    return line;
}