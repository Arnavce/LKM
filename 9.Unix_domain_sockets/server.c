// server.c
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<string.h>

#define SOCKET_PATH "./tmp/unix/sock"

int main(void) {
    int server_fd, client_socket;
    struct sockaddr_un address;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address));  // IMPORTANT
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    unlink(SOCKET_PATH);  // Remove old socket file if exists

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s\n", SOCKET_PATH);

    while (1) {
        client_socket = accept(server_fd, NULL, NULL);
        if (client_socket == -1) {
            perror("accept failed");
            continue;
        }

        char buffer[128] = {0};
        int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_read > 0) {
            printf("Received message: %s\n", buffer);
            send(client_socket, "Hello from server", 17, 0);
        }
        close(client_socket);
    }

    close(server_fd);
    return 0;
}
