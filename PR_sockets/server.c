#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main(int argc, char const *argv[]) {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char message[1024];

    WSAStartup(MAKEWORD(2, 2), &wsa);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 3);

    new_socket = accept(server_fd, (struct sockaddr *)&address, (int *)&addrlen);


    while (1) 
	{
        int valread = recv(new_socket, buffer, 1024, 0);
        buffer[valread] = '\0';
        printf("Client Message: %s", buffer);

        printf("Enter message to send: ");
        fgets(message, sizeof(message), stdin);

        send(new_socket, message, strlen(message), 0);

        memset(buffer, 0, sizeof(buffer));
        memset(message, 0, sizeof(message));
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}