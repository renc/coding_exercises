// most simple one: 
// https://www.bogotobogo.com/cplusplus/sockets_server_client.php
// https://www.bogotobogo.com/cplusplus/sockets_server_client_2.php
// https://www.bogotobogo.com/cplusplus/sockets_server_client_3.php
// https://www.geeksforgeeks.org/socket-programming-cc/ 
// 
// 2021/3/15 

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // in_addr
#include <netdb.h>
#include <arpa/inet.h> // inet_aton, inet_ntoa

void error(const char* msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s s port\n", argv[0]);
        fprintf(stderr, "Usage: %s c host port\n", argv[0]);
        exit(1);
    }
    
    if (*argv[1] == 's')
    {
        // as a server 

        // create a socket
        // socket(int domain, int type, int protocol)
        int fdServerSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (fdServerSocket < 0)
            error("ERROR opening socket");

        struct sockaddr_in serv_addr;
        // clear address structure
        memset((char*)&serv_addr, 0, sizeof(serv_addr));
        int portno = atoi(argv[2]);
        // setup the host_addr structure for use in bind call 
        // server byte order
        serv_addr.sin_family = AF_INET;
        // automatically be filled with current host's IP address
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        // convert short integer value for port must be converted into network byte order
        serv_addr.sin_port = htons(portno);

        // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
        // bind() passes file descriptor, the address structure, 
        // and the length of the address structure
        // This bind() call will bind  the socket to the current IP address on port, portno
        if (bind(fdServerSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
            error("ERROR on binding");

        // This listen() call tells the socket to listen to the incoming connections.
        // The listen() function places all incoming connection into a backlog queue
        // until accept() call accepts the connection.
        // Here, we set the maximum size for the backlog queue to 5.
        if (listen(fdServerSocket, 5) < 0)
        {
            error("Error listen");
        }

        struct sockaddr_in cli_addr;
        // The accept() call actually accepts an incoming connection
        socklen_t clilen = sizeof(cli_addr);

        // This accept() function will write the connecting client's address info 
        // into the the address structure and the size of that structure is clilen.
        // The accept() returns a new socket file descriptor for the accepted connection.
        // So, the original socket file descriptor can continue to be used 
        // for accepting new connections while the new socker file descriptor is used for
        // communicating with the connected client.
        int fdClientSocket = accept(fdServerSocket, (struct sockaddr*)&cli_addr, &clilen);
        if (fdClientSocket < 0)
            error("ERROR on accept");

        printf("server: got connection from %s port %d\n",
            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        // This send() function sends the 13 bytes of the string to the new socket
        send(fdClientSocket, "Hello, world!\n", 13, 0); // renc, \n is not sent when 13, \n is sent when 14.
        printf("server: after send.\n"); // renc, as we can see client does not print its recv msg until finish its get &sending.
        char buffer[256];
        memset(buffer, 0, 256);

        int n = read(fdClientSocket, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message=[%s]\n", buffer);

        close(fdClientSocket); 
    }
    else if (*argv[1] == 'c')
    {
        // as a client 
         
        if (argc < 4) {
            fprintf(stderr, "usage %s c hostname port\n", argv[0]);
            exit(0);
        }
        int portno = atoi(argv[3]);

        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");
        struct hostent* server = gethostbyname(argv[2]);
        if (server == NULL) {
            fprintf(stderr, "ERROR, no such host\n");
            exit(0);
        }
        struct sockaddr_in serv_addr;
        memset((char*)&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
            error("ERROR connecting");

        char buffer[256];
        printf("Please enter the message: ");
        memset(buffer, 0, 256);
        fgets(buffer, 255, stdin);
        int n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
        
        memset(buffer, 0, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("%s\n", buffer);
        close(sockfd);
    } 
    else
    {
        std::cerr << "Unknown mode.\n"; 
    }

    return 0;
}