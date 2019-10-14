#include <string>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
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

};