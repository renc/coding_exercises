// similar of ey1_socket.cpp, ey2_socket2select.cpp 
// extend to use select to handle multiple clients
// http://www.beej.us/guide/bgnet/html/
// 2021/3/27 

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
    std::cerr << msg << " [" << strerror() << "]" << std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s s port\n", argv[0]);
        fprintf(stderr, "Usage: %s c host port\n", argv[0]);
        exit(1);
    }
    
    if (strcmp(argv[1], "s" == 0)
    {
        // as a server 
        struct addrinfo hints, * res; 

        // first, load up address structs with getaddrinfo():
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
        hints.ai_socktype = SOCK_STREAM; // tcp, not udp
        hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
         
        getaddrinfo(NULL, argv[2], &hints, &res);

        // make a socket:
        int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        
        int yes = 1;
        //char yes='1'; // Solaris people use this
        // lose the pesky "Address already in use" error message
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes) ) == -1) 
        {
            std::cerr << "setsockopt" << std::endl;
            exit(1);
        }
        
        // bind it to the port we passed in to getaddrinfo():
        if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
            error("ERROR on binding");
        } 
         
        // This listen() call tells the socket to listen to the incoming connections.
        // The listen() function places all incoming connection into a backlog queue
        // until accept() call accepts the connection.
        // Here, we set the maximum size for the backlog queue to 5.
        if (listen(sockfd, 5) < 0)
        {
            error("Error listen");
        }

        std::cout << "Waiting for client connections ...\n"; 
        // so far the code is similar to ey1_socket for setting the sever.  
        
        struct sockaddr_storage their_addr;
        socklen_t addr_size;
        addr_size = sizeof their_addr;
        int new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
        
        const char* msg = "Beej was here!";
        int len = strlen(msg);
        int bytes_sent = send(new_fd, msg, len, 0);
         
        getchar();
        close(sockfd); 
    }
    else if ( strcmp(argv[1], "sp" ) == 0 )
    {
        // as a server, polling (i think it is kind of similar to select() )
  
    } 
    else if (strcmp(argv[1], "ss") == 0) {
        // as a server, using select() 
    }
    else
    {
        std::cerr << "Unknown mode.\n"; 
    }

    return 0;
}