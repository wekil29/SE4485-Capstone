/*
    File updated on 11/18/22 by Justin Jones. 

    summary:    seccomp functionality added for client. installation and demo of seccomp filter

    changes:    include statements for seccomp.h and seccomp-bpf.h added. arguments for main added. 
                The seccomp portion (line 77+) was added.
*/


#include <arpa/inet.h> // inet_addr()
#include <strings.h> // bzero()
#include "Config.h"
#include "seccomp.h"
#include "seccomp-bpf.h"

#define MAX 200
#define IP_ADDR "127.0.0.1"

void getMessage(int sockfd)
{
    // Read message from sockfd
    char buff[MAX];
    read(sockfd, buff, sizeof(buff));
    printf("Received from the server:\n\t%s\n", buff);
}

int connectSocket(int port_num)
{
    int sockfd;
    struct sockaddr_in servaddr, cli;

    // Socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Client failed to create socket\n");
        exit(0);
    }
    else
        printf("Client created socket\n");
    bzero(&servaddr, sizeof(servaddr));

    // Configures the client's port number and IP address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port_num);
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDR);

    // Connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Client failed to connect to the server\n");
        exit(0);
    }
    else
        printf("Client connected to the server\n");

    return sockfd;
}

int main(int argc, char** argv)
{
    // Get the socket
    char IP_ADDRESS[] = "127.0.0.1";

    // Get port
    std::string file_name = ".config";
    Config config(file_name);
    int port_num = config.getPortNum();

    int sockfd = connectSocket(port_num);
    // Receive a message from the server
    getMessage(sockfd);
 
    // Close the socket
    printf("Closing client socket\n");
    close(sockfd);

    //seccomp
    int before_allow = 1;
    int after_allow = 1;

    signal(SIGSYS, handle_sigsys);
    parse_args(argc, argv, &before_allow, &after_allow);

    if (install_syscall_filter( before_allow )) 
    {
        printf("filter install failure: original\n");
        exit(4);
    }

    pid_t forkStatus;
    forkStatus = fork();

    if (forkStatus == 0) 
    {
        signal(SIGSYS, handle_sigsys);
        printf("-->  Hello from the child. Installing filter now.\n");
        if (install_syscall_filter( after_allow )) {
            printf("filter install failure: child\n");
            exit(5);
        }
        printf("-->  Child sleeping.\n");
        sleep(2);
        printf("-->  Can child get pid?.\n");
        pid_t cpid = getpid();
        printf("-->  childpid = %d.\n", cpid);
        printf("-->  Child terminating.\n");
    }
    else if (forkStatus != -1) 
    {
        printf("[Parent process.]\n");
        wait(NULL);
        pid_t ppid = getpid();
        printf("[as the parent I can get my PID. See pid=%d.]\n", ppid);
        printf("[Parent terminating]\n");

    } else {
        perror("Error calling fork()");
    }

}
