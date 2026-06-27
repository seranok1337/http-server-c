#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "./include/signal_error.h"
#include "./include/file.h"
#include "./include/parse_request.h"

#define PORT 1337

int main(void)
{
    int server_sock_fd, incoming_sock_fd;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    FILE *document_fptr = init_file("./static/index.html");

    char buffer[10000];

    char header[100] = "HTTP/1.1 200 OK\r\n\n";

    strcpy(buffer, header);

    while (!feof(document_fptr))
    {
        char *line = read_line(document_fptr);

        strncat(buffer, line, sizeof(line));

        free(line);
    }

    if ((server_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        signal_error("failed to create server socket");
    }

    int reuse = 1;
    setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    socklen_t server_addr_len = sizeof(server_addr);

    if (bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        signal_error("failed to bind socket to network server_addr");
    }

    if (listen(server_sock_fd, 1) < 0)
    {
        signal_error("failed to start listening for incoming connections");
    }

    printf("Listening on port %d\n", PORT);

    char incoming_buffer[10000];

    while (1)
    {
        if ((incoming_sock_fd = accept(server_sock_fd, NULL, NULL)) < 0)
        {
            signal_error("failed to accept incoming connection");
        }

        size_t document_len = strlen(buffer);

        if ((write(incoming_sock_fd, buffer, document_len)) < 0)
        {
            signal_error("failed to write to incoming socket");
        }
        
        if ((read(incoming_sock_fd, incoming_buffer, sizeof(incoming_buffer))) < 0)
        {
            signal_error("failed to read buffer from incoming socket");
        }
        
        http_req_t incoming_req = parse_request(incoming_buffer, sizeof(incoming_buffer));
        
        printf("%s\n", incoming_req.method);

        printf("%s\n", incoming_req.path);

        close(incoming_sock_fd);
    }

    close(server_sock_fd);

    return 0;
}