#include "./struct/http_request.h"
#include <string.h>
#include <stdio.h>

http_req_t parse_request(char* buffer, size_t size)
{
    http_req_t request;

    request.buffer = buffer;
    request.buffer_len = size;

    char *token = strtok(buffer, "   ");

    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
            request.method = token;
        if (i == 1)
            request.path = token;

        token = strtok(NULL, " - ");
    }

    return request;
}