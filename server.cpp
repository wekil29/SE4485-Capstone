#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "ServerConfig.h"
#define SA struct sockaddr

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
    // Assign IP, PORT
    std::string file_name = ".config";
    ServerConfig serverConfig(file_name);
    int port_num = serverConfig.getPortNum();
   
    connfd = bindSocket(port_num);

    // Send a message to the client
    ServerMessageService(connfd);
   
    // After sending a message close the socket
    close(connfd);
    return 0;
}
int bindSocket(int port_num)
{
    int sockfd, connfd, length;
    socklen_t addrlen;
    struct sockaddr_in servaddr, addr;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create socket\n");
        exit(0);
    }
    else
        printf("Socket successfully created\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Bind socket to any IP
    servaddr.sin_port = htons(port_num);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Failed to bind socket\n");
        exit(0);
    }
    else
        printf("Socket successfully bound\n");

    if ((listen(sockfd, 3)) != 0) {  // Second parameter is the backlog, max length of pending connection queue
        printf("Failed to listen to socket");
        exit(0);
    }
    else
        printf("Listening to socket");

    // Accept the data packet from the client and complete the connection
    addrlen = sizeof(addr);
    connfd = accept(sockfd, (SA*)&addr, &addrlen);
    if (connfd < 0) {
        printf("Failed to accept client\n");
        exit(0);
    }
    else
        printf("Client accepted by server\n");

    return connfd;
}
