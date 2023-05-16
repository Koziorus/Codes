#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main(int argc, char const *argv[]) 
{
    WSADATA wsa;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 

    char message[1024];
    char response[1024];
    while (1) {
        printf("Enter message to send: ");
        fgets(message, sizeof(message), stdin);

        send(sock, message, strlen(message), 0);

        int valread = recv(sock, response, 1024, 0);
        response[valread] = '\0';
        printf("Server Response: %s", response);

        memset(message, 0, sizeof(message));
        memset(response, 0, sizeof(response));
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}