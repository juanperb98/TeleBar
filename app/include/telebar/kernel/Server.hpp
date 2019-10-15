#ifndef _TELEBAR_KERNEL_SERVER_
#define _TELEBAR_KERNEL_SERVER_

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

#include <telebar/entity/User.hpp>

typedef std::string (*handlerType)(std::string);

class Server{

    struct serverClient {
        User client;
        int socket_fd;
    };

private:
    int server_fd;
    sockaddr_in server;
    std::string buffer;
    std::vector<serverClient> clients;
    fd_set clients_fds;
    handlerType handler;
    int clientCap;

public:
    Server(int portno, handlerType requestHandler);
    int getClientCap() const;
    void setClientCap(int cap);
    int getNumberOfClients() const;

public:
    bool handleNextConnection();

private:
    bool createNewSessionForClient();
    bool handleConnectionFromClient(int client_fd);
    bool handleClientExit(int client_fd);

};

#endif


