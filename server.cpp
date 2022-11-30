#include "Config.h"
#include "seccomp.h"

void sendMessage(int connfd) {
    char message[] = "This is a message from the server";

    // Send a message to the client
    write(connfd, message, sizeof(message));
    printf("Sent message to client:\n\t%s\n", message);
}

int bindSocket(int port_num) {
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind socket to any IP
    servaddr.sin_port = htons(port_num);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
        printf("Server failed to bind socket\n");
        exit(0);
    }
    else
        printf("Server bound socket\n");

    if ((listen(sockfd, 3)) != 0) { // Second parameter is the backlog, max length of pending connection queue
        printf("Server failed to listen to socket\n");
        exit(0);
    }
    else
        printf("Server listening to socket\n\n");

    return sockfd;
}

int acceptClient(int sockfd) {
    int connfd;
    socklen_t addrlen;
    struct sockaddr_in addr;

    // Accept the data packet from the client and complete the connection
    addrlen = sizeof(addr);
    connfd = accept(sockfd, (SA *)&addr, &addrlen);
    if (connfd < 0) {
        printf("Server failed to accept client\n");
        exit(0);
    }
    else
        printf("Server accepted client\n");

    return connfd;
}

void set_seccomp_filters() {
    install_filter(__NR_open, AUDIT_ARCH_X86_64, EPERM);
    // Add more syscall filters below as needed
}

// Driver function
int main() {
    int sockfd, connfd;
    // Assign IP, PORT
    std::string file_name = ".config";
    Config config(file_name);
    int port_num = config.getPortNum();

    // Seccomp
    set_seccomp_filters();

    // Binding to server socket
    sockfd = bindSocket(port_num);

    // Opens server listener after closing socket
    while (true) {
        // Accepts client socket
        connfd = acceptClient(sockfd);

        // Send a message to the client
        sendMessage(connfd);

        // After sending a message terminate the socket and open new socket
        printf("Closing connection socket\n");
        close(connfd);
    }

    return 0;
}
