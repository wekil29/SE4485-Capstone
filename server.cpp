#include "Config.h"
#include "seccomp.h"

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
        printf("Server failed to create socket\n");
        exit(0);
    }
    else
        printf("Server created socket\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Bind socket to any IP
    servaddr.sin_port = htons(port_num);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Server failed to bind socket\n");
        exit(0);
    }
    else
        printf("Server bound socket\n");

    if ((listen(sockfd, 3)) != 0) {  // Second parameter is the backlog, max length of pending connection queue
        printf("Server failed to listen to socket\n");
        exit(0);
    }
    else
        printf("Server listening to socket\n\n");


    // Accept the data packet from the client and complete the connection
    addrlen = sizeof(addr);
    connfd = accept(sockfd, (SA*)&addr, &addrlen);
    if (connfd < 0) {
        printf("Server failed to accept client\n");
        exit(0);
    }
    else
        printf("Server accepted client\n");

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

    //seccomp
      printf("hey there!\n");

      install_filter(__NR_write, AUDIT_ARCH_X86_64, EPERM);

      printf("something's gonna happen!!\n");
      printf("it will not definitely print this here\n");
    
//     signal(SIGSYS, handle_sigsys);
//     parse_args(argc, argv, &before_allow, &after_allow);
//     if (install_syscall_filter(before_allow)) 
//     {
//         printf("filter install failure\n");
//         exit(4);
//     } // TODO test whether seccomp section needs to be put here or inside of the while(true) loop
    
    // Opens server listener after closing socket
    while(true) {
        // Binding to server socket
        connfd = bindSocket(port_num);

        // Send a message to the client
        sendMessage(connfd);

        // After sending a message terminate the socket and open new socket
        printf("Closing server socket\n");
        close(connfd);
    }

    return 0;
}
