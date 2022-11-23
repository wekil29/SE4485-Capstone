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
//#include "seccomp-bpf.h"

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

void set_seccomp_filters()
{
   install_filter(__NR_open, AUDIT_ARCH_X86_64, EPERM);
   // Add more syscall filters below as needed
}

int main()
{
    // Setting up variables
    //int before_allow = 1;   // seccomp variable
    //int after_allow = 1;    // seccomp variable
    char IP_ADDRESS[] = "127.0.0.1";    // Get the socket
    std::string file_name = ".config";  // Name of config file with port number
    int port_num;                       // Port number
    int sockfd;                         // File descriptor of socket

    // Get port
    Config config(file_name);
    port_num = config.getPortNum(); // Must run before seccomp filter is installed so that it does not get blocked

    //seccomp
    set_seccomp_filters();

    sockfd = connectSocket(port_num);
    // Receive a message from the server

        getMessage(sockfd);

    // Close the socket
    printf("Closing client socket\n");
    close(sockfd);

    // Attempt the restricted syscall (open)
    std::string restricted_file_name = "seccomp.h";
    std::string restricted_result = "open syscall was blocked."; // Expected: Should remain unchanged since overriding reads are blocked by seccomp
    std::fstream testStream (restricted_file_name, std::fstream::in);
    // Attempt to read 5th line from file; should be blocked by seccomp
    for (int i = 0; i < 5; i++)
    {
       std::getline (testStream, restricted_result);
    }
    printf("Result of restricted syscall: %s \n", restricted_result.c_str());
}
