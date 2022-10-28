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
#define IP_ADDR "127.0.0.1"
#define SA struct sockaddr

void ClientMessageService(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int connectSocket(int port_num, char* ip_addr)
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

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_addr);
    servaddr.sin_port = htons(port_num);

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
    int sockfd = connectSocket(PORT, IP_ADDRESS);
    // Receive a message from the server
    ClientMessageService(sockfd);
 
    // Close the socket
    close(sockfd);
}