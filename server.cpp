#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "ServerConfig.h"
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int sockfd, connfd;
unsigned int len;
struct sockaddr_in servaddr, cli;

int bindSocket(int port_num);

// Function designed for chat between client and server
void ServerMessageService(int connfd)
{
    // char buff[MAX];
    char message[] = "This is a message from the server";

    // Send a message to the client
    write(connfd, message, sizeof(message));
}

// Driver function
int main()
{
    int connfd;
    // assign IP, PORT
    std::string file_name = ".config";
    ServerConfig serverConfig(file_name);
    //serverConfig = ServerConfig(file_name);
    int port_num = serverConfig.getPortNum();

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port_num);
   
    connfd = bindSocket(port_num);

    // Function for chatting between client and server
    ServerMessageService(connfd);
   
    // After chatting close the socket
    close(sockfd);
    return 0;
}
int bindSocket(int port_num)
{
    int sockfd, connfd, length;
    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create socket\n");
        exit(0);
    }
    else
        printf("Socket successfully created\n");
    bzero(&servaddr, sizeof(servaddr));

    // Binding newly created socket to given IP and verification
    struct sockaddr_in servaddr, cli;
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Failed to bind socket\n");
        exit(0);
    }
    else
        printf("Socket successfully bound\n");

    // Accept the data packet from client and verification
    len = sizeof(cli);
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Failed to accept client\n");
        exit(0);
    }
    else
        printf("Client accepted by server\n");

    return connfd;
}
