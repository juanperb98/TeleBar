#include <telebar/kernel/Client.hpp>

Client::Client(const std::string& addr, int portno) {
    this->socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    // TODO: if error

    this->server_dir_.sin_family = AF_INET;
    this->server_dir_.sin_addr.s_addr = inet_addr(addr.c_str());
    this->server_dir_.sin_port = htons(portno);

    this->server_dir_len_ = sizeof(this->server_dir_);

    {
        int ret_val;
        ret_val = connect(
                this->socket_fd_,
                (sockaddr*) &this->server_dir_,
                this->server_dir_len_
        );
        // TODO: finish error
        if (ret_val == -1){
            fprintf(stderr, "ERROR TODO");
        }
    }

}

bool Client::sendMessage(const std::string& payload) {
    send(
            this->socket_fd_,
            payload.c_str(),
            strlen(payload.c_str()),
            0
    );
    return true;
}

std::string Client::listen() {
    char buffer_c[255];
    bzero(buffer_c, 255);
    recv(
            this->socket_fd_,
            buffer_c,
            255,
            0
    );

    return std::string(buffer_c);
}

bool Client::closeConnection() {
    this->sendMessage("EXIT");
    return true;
}
