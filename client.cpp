#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define MAX 200
#define PORT 8090
#define IP_ADDR "127.0.0.1"
#define SA struct sockaddr

void ClientMessageService(int sockfd)
{
    char packet[] = "M";
    write(sockfd, packet, sizeof(packet));

    char buff[MAX];
    bzero(buff, sizeof (buff));
    read(sockfd, buff, sizeof(buff));
    printf("Received from the server:\n\t%s\n", buff);
}

int connectSocket(int port_num)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // Socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create socket\n");
        exit(0);
    }
    else
        printf("Created socket\n");
    bzero(&servaddr, sizeof(servaddr));

    // Configures the client's port number and IP address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDR);

    // Connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Failed to connect to the server\n");
        exit(0);
    }
    else
        printf("Connected to the server\n");

    return sockfd;
}

int main()
{
    // Get the socket
    char IP_ADDRESS[] = "127.0.0.1";
    int sockfd = connectSocket(PORT);
    // Receive a message from the server
    ClientMessageService(sockfd);
 
    // Close the socket
    printf("Closing client socket\n");
    close(sockfd);
}