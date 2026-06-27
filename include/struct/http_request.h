#include <stdio.h>
#pragma once

typedef struct http_request
{
    char *method;
    char *path;
    char *buffer;
    size_t buffer_len;
} http_req_t;
