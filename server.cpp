#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "Config.h"
#define SA struct sockaddr

// Function designed for chat between client and server
void sendMessage(int connfd)
{
    // char buff[MAX];
    char message[] = "This is a message from the server";

    // Send a message to the client
    write(connfd, message, sizeof(message));
    printf("Sent message to client:\n\t%s\n", message);
}

int bindSocket(int port_num)
{
    int sockfd, connfd, length;
    socklen_t addrlen;
    struct sockaddr_in servaddr, addr;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    if (sockfd == -1) {
        printf("Failed to create socket\n");
        exit(0);
    }
    else
        printf("Created socket\n");
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
        printf("Bound socket\n");

    if ((listen(sockfd, 3)) != 0) {  // Second parameter is the backlog, max length of pending connection queue
        printf("Failed to listen to socket\n");
        exit(0);
    }
    else
        printf("Listening to socket\n\n");

    // Fancy loop animation
    for(int loop = 0; loop < 2; ++loop) {
        for(int each = 0; each < 4; ++each) {
            printf("\r %.*s   \b\b\b", each, "...");
            fflush(stdout); // force printing as no newline in output
            sleep(1);
        }
    }
    printf("\n");

    // Accept the data packet from the client and complete the connection
    addrlen = sizeof(addr);
    connfd = accept(sockfd, (SA*)&addr, &addrlen);
    if (connfd < 0) {
        printf("Failed to accept client\n");
        exit(0);
    }
    else
        printf("Accepted client\n");

    return connfd;
}

// Driver function
int main()
{
    int connfd;
    // Assign IP, PORT
    std::string file_name = ".config";
    Config config(file_name);
    int port_num = config.getPortNum();

    connfd = bindSocket(port_num);

    // Send a message to the client
    sendMessage(connfd);

    // After sending a message close the socket
    printf("Closing server socket\n");
    close(connfd);
    return 0;
}