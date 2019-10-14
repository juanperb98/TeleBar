#include <telebar/kernel/Client.hpp>

Client::Client(std::string addr, int portno) {
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // TODO: if error

    this->server_dir.sin_family = AF_INET;
    this->server_dir.sin_addr.s_addr = inet_addr(addr.c_str());
    this->server_dir.sin_port = htons(portno);

    this->server_dir_len = sizeof(this->server_dir);

    {
        int ret_val;
        ret_val = connect(
                this->socket_fd,
                (sockaddr*) &this->server_dir,
                this->server_dir_len
        );

        // TODO: check ret val
    }

}

bool Client::sendMessage(const std::string& payload) {
    send(
            this->socket_fd,
            payload.c_str(),
            sizeof(payload.c_str()),
            0
    );
    return true;
}

std::string Client::listen() {
    char buffer_c[255];
    bzero(buffer_c, 255);
    recv(
            this->socket_fd,
            buffer_c,
            255,
            0
    );

    return std::string(buffer_c);
}
