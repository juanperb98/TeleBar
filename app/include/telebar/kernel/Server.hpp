#ifndef _TELEBAR_KERNEL_SERVER_
#define _TELEBAR_KERNEL_SERVER_

#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

#include <telebar/entity/User.hpp>


/**
 * All the handlers must follow this type
 */
typedef const std::string& (*handlerType)(std::string);


/**
 * A connection oriented with client sessions
 * The client session only saves the connection itself, but the rest is STATELESS, no user cookie, or any other kind
 * of data.
 * This server does not handle any kind of user validation, that is why the handler is present, the idea for this
 * class is to be as modular as it can be.
 */
class Server {

    struct serverClient {
        // User client = User(); // unused, but can be useful
        int socket_fd = -1;
    };

private:
    int server_fd_;
    sockaddr_in server_{};
    int bufferSize_{};
    std::vector<serverClient> clients_; // list of clients (basic information) and its file descriptors
    fd_set clients_fds_{};
    handlerType handler_;
    int clientCap_;

public:
    Server(int portno, handlerType requestHandler);
    int getClientCap() const;
    void setClientCap(int cap);
    int getNumberOfClients() const;

public:
    /**
     * handles the next connection of the client
     * @return true if all is ok, false if an error has happen
     */
    bool handleNextConnection();

private:
    /**
     * handles a new connection from an unknown client
     * @return true if all is ok, false if an error has happen
     */
    bool createNewSessionForClient();

    /**
     * handles a connection from an existing client
     * @param client_fd file descriptor for the actual client
     * @return true if all is ok, false if an error has happen
     */
    bool handleConnectionFromClient(int client_fd);

    /**
     * handles an exit request from a client
     * @param client_fd file descriptor for the actual client
     * @return true if all is ok, false if an error has happen
     */
    bool handleClientExit(int client_fd);

};

#endif


