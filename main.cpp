#include <stdio.h>

int main(int argc, char** argv) {
// seccomp runs here

    if(fork() == 0)
    { 
        // server starts here
        startServer();
    }
    else
        // client starts here
        startClient();

    return 0;
    }

void startServer() {


}

void startClient() {


}