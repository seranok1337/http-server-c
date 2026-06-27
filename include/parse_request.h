#pragma once 

#include "./struct/http_request.h"

http_req_t parse_request(char* buffer, size_t size);

