// Based on ey1_socket.cpp
// extend to use select to handle multiple clients
// 2021/3/16 

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
        // socket(int domain, int type, int protocol), what kind of socket u want
        int master_socket = socket(PF_INET, // some may use AF_INET here, almost the same
            SOCK_STREAM, 0);
        if (master_socket < 0)
            error("ERROR opening socket");

        //set master socket to allow multiple connections , 
         //this is just a good habit, it will work without this 
        // Setting the socket option SO_REUSEADDR of level SOL_SOCKET. Otherwise, 
        // stale connections might hinder the program from re-binding to the desired address after quitting and restarting the server.
        int opt = 1;
        if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
            sizeof(opt)) < 0)
        {
            error("setsockopt"); 
        }


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
        if (bind(master_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
            error("ERROR on binding");

        // This listen() call tells the socket to listen to the incoming connections.
        // The listen() function places all incoming connection into a backlog queue
        // until accept() call accepts the connection.
        // Here, we set the maximum size for the backlog queue to 5.
        if (listen(master_socket, 5) < 0)
        {
            error("Error listen");
        }

        std::cout << "Waiting for client connections ...\n"; 
        // so far the code is same as ey1_socket for setting the sever.  
        
        int addrlen = sizeof(serv_addr);
        // set of socket descriptors
        fd_set readfds; 
        int max_sd = -1;
        int max_clients = 30;
        int client_socket[max_clients] = { 0 };

        while (true) {
            // clear the socket set 
            FD_ZERO(&readfds); 

            // add server socket to the set     <-- renc, i'm a bit confused to add everytime.
            FD_SET(master_socket, &readfds);
            max_sd = master_socket; 
             
            //add child sockets to the set 
            for (int i = 0; i < max_clients; i++)
            {
                //socket descriptor 
                int sd = client_socket[i];

                //if valid socket descriptor then add to read list 
                if (sd > 0)
                    FD_SET(sd, &readfds);

                //highest file descriptor number, need it for the select function 
                if (sd > max_sd)
                    max_sd = sd;
            }

            //wait for an activity on one of the sockets , timeout is NULL , 
            //so wait indefinitely 
            int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

            if ((activity < 0) //&& (errno != EINTR)
                )
            {
                printf("select error");
            }

            //If something happened on the master socket , 
            //then its an incoming connection 
            if (FD_ISSET(master_socket, &readfds))
            {
                // how to accept new client connection
                // this part is same as w/o using select
                int new_socket = -1;
                struct sockaddr_in cli_addr;
                socklen_t clilen = sizeof(cli_addr);
                if ((new_socket = accept(master_socket,
                    (struct sockaddr*)&cli_addr, (socklen_t*)&clilen)) < 0)
                {
                    perror("accept"); 
                }

                //inform user of socket number - used in send and receive commands 
                printf("New connection , socket fd is %d , ip is : %s , port : %d \n" 
                    , new_socket , inet_ntoa(cli_addr.sin_addr) , ntohs(cli_addr.sin_port));

                //send new connection greeting message 
                const char* message = "Hello message from server.";
                if (send(new_socket, message, strlen(message), 0) != strlen(message))
                {
                    error("Error: send");
                }
                std::cout << "[server s][" << message << "] len=" << strlen(message) << std::endl;

                //add new socket to array of sockets 
                for (int i = 0; i < max_clients; i++)
                {
                    //if position is empty 
                    if (client_socket[i] == 0)
                    {
                        client_socket[i] = new_socket;
                        printf("Adding to list of sockets as %d\n", i);
                        break;
                    }
                }
            }

            //else its some IO operation on some other socket 
            for (int i = 0; i < max_clients; i++)
            {
                int sd = client_socket[i];

                if (FD_ISSET(sd, &readfds))
                {
                    //Check if it was for closing , and also read the 
                    //incoming message
                    int valread = 0; 
                    char buffer[1024] = { 0 };
                    if ((valread = read(sd, buffer, 1024)) == 0)
                    {
                        //Somebody disconnected , get his details and print 
                        struct sockaddr_in cli_addr;
                        socklen_t clilen = sizeof(cli_addr);
                        getpeername(sd, (struct sockaddr*)&cli_addr, (socklen_t*)&clilen);
                        printf("Host disconnected, ip %s , port %d. \n",
                            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

                        //Close the socket and mark as 0 in list for reuse 
                        close(sd);
                        client_socket[i] = 0;
                    }

                    //Echo back the message that came in 
                    else if (valread > 0)
                    {
                        //set the string terminating NULL byte on the end 
                        //of the data read 
                        buffer[valread] = '\0';
                        std::cout << "[server r][" << buffer << "] len=" << valread << std::endl;
                        send(sd, buffer, strlen(buffer), 0);
                    }
                    else {
                        // valread == -1, why ?
                        // maybe the client has closed, while we do not know?
                    }
                }
            }
        } // end of while 
         
        
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
        
        int n = -1;
        char buffer[256];
        memset(buffer, 0, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("[client r][%s] len=%d\n", buffer, n);

        //const char* sHello = "Hi.";
        char sHello[256];
        memset(sHello, 0, 256);
        strcpy(sHello, "Hello from client.");
        n = write(sockfd, sHello, strlen(sHello));
        if (n < 0) {
            error("ERROR writing to socket");
        } {
            std::cout << "[client w][" << sHello << "] len=" << n<< ".\n";
        }
        std::cout << "[client] to close socket.\n";
        close(sockfd);
    } 
    else
    {
        std::cerr << "Unknown mode.\n"; 
    }

    return 0;
}