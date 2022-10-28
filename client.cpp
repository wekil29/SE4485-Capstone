#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void ClientConfig(sockaddr_in servaddr);
void setPortNum(sockaddr_in servaddr);
void setIPAddr(sockaddr_in servaddr);

void ClientMessageService(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
 
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // Configures the client's port number and IP address 
    ClientConfig(servaddr);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // function for chat
    ClientMessageService(sockfd);
 
    // close the socket
    close(sockfd);
}

void ClientConfig(sockaddr_in servaddr) {
    setPortNum(servaddr);
    setIPAddr(servaddr);
}

void setPortNum(sockaddr_in servaddr) {
    // assign PORT from config file
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
}

void setIPAddr(sockaddr_in servaddr) {
    // assign IP to 127.0.0.1
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}