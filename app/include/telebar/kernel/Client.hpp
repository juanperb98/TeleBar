#ifndef _TELEBAR_KERNEL_CLIENT_
#define _TELEBAR_KERNEL_CLIENT_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

class Client {
private:
    int socket_fd;
    sockaddr_in server_dir;
    socklen_t server_dir_len;
public:
    Client(std::string addr, int portno);

    bool sendMessage(const std::string& payload);

    std::string listen();

    bool closeConnection();

};


#endif
