// client.c
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<string.h>

#define SOCKET_PATH "./tmp/unix/sock"

int main(void) {
    int sock;
    struct sockaddr_un addr;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    send(sock, "Hello from client", 17, 0);

    char buffer[128] = {0};
    int bytes_read = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_read > 0) {
        printf("Received: %s\n", buffer);
    }

    close(sock);
    return 0;
}
