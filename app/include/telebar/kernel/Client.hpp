#ifndef _TELEBAR_KERNEL_CLIENT_
#define _TELEBAR_KERNEL_CLIENT_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>


/**
 * This simple client starts a connection to a server, one this is made (automatic) it can send and receive payloads
 * to and from that server.
 */
class Client {
private:
    int socket_fd_;
    sockaddr_in server_dir_{};
    socklen_t server_dir_len_;
public:
    Client(const std::string& addr, int portno);

    bool sendMessage(const std::string& payload);

    std::string listen();

    bool closeConnection();

};


#endif
