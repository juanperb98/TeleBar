#include <telebar/kernel/Server.hpp>


Server::Server(int portno, handlerType requestHandler) {
    this->orm_ = new ORM(DB_NAME);
    this->clientCap_ = 50;
    this->bufferSize_ = BUFFER_SIZE;

    this->server_.sin_port = htons(portno);
    this->server_.sin_family = AF_INET;
    this->server_.sin_addr.s_addr = htonl(INADDR_ANY);
    this->handler_ = requestHandler;


    this->server_fd_ = socket(
            this->server_.sin_family,
            SOCK_STREAM,
            0
    );

    int on=1;
    setsockopt(this->server_fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    {
        int bind_payload = bind(
                this->server_fd_,
                (struct sockaddr*) &this->server_,
                sizeof(this->server_)
        );

        if (bind_payload == -1) {
            fprintf(stderr, "ERROR: unable to bind socket.\n");
            close(this->server_fd_);
            exit(1);
        }
    }
    {
        int ret_val = listen(this->server_fd_, 10);

        if (ret_val == -1) {
            fprintf(stderr, "ERROR, could not set the socket to listen.\n");
            close(this->server_fd_);
            exit(2);
        }
    }

    FD_ZERO(&this->clients_fds_);
    FD_SET(this->server_fd_, &this->clients_fds_);
    FD_SET(0,&this->clients_fds_);
    FD_CLR(STDIN_FILENO, &this->clients_fds_);
}

bool Server::handleNextConnection() {
    fd_set clients_fds_aux = this->clients_fds_;

    int output;
    output = select(
        FD_SETSIZE,
        &clients_fds_aux,
        nullptr,
        nullptr,
        nullptr
    );

    if (output <= 0) // there is no requests
        return false;


    for (int i = 0; i < FD_SETSIZE; ++i) {
        if (!FD_ISSET(i, &clients_fds_aux)) continue;

        // new client is requesting a session
        if (i == this->server_fd_)
            return this->createNewSessionForClient();

        // already connected client is sending a payload
        else
            return this->handleConnectionFromClient(i);
    }
    return true;
}

bool Server::createNewSessionForClient() {
    std::cout<<"accepting connection\n";

    serverClient client;

    int client_addr;
    socklen_t client_addr_length = sizeof(client_addr);

    // gets the request
    client.socket_fd = accept(
            this->server_fd_,
            (sockaddr*) &client_addr,
            &client_addr_length
    );

    // checks if the fd was created successfully
    if (client.socket_fd == -1)
        return false;

    // checks if the client cap has been reached
    if (this->getNumberOfClients() > this->getClientCap()) {
        char buffer[this->bufferSize_];
        bzero(buffer, this->bufferSize_);
        strcpy(buffer, "Sorry, but the client cap has been reached, please, try again later.");
        send(
                client.socket_fd,
                buffer,
                strlen(buffer),
                0
        );
        close(client.socket_fd);
        return true;
    }

     // lets get that client connected

     // adds the client to the FD set and client list with out the token_, as he must log in or create an User to
     // continue
    this->clients_.push_back(client);
    FD_SET(client.socket_fd, &clients_fds_);

    char buffer[this->bufferSize_];
    bzero(buffer, this->bufferSize_);
    strcpy(buffer, "INFO,Welcome, please, log in or register now.");
    send(
            client.socket_fd,
            buffer,
            strlen(buffer),
            0
    );

    return true;
}

bool Server::handleConnectionFromClient(int client_fd) {
    // int socket_fd = -1;
    // int client_addr;
    // socklen_t client_addr_length = sizeof(client_addr);

    char buffer[this->bufferSize_];
    bzero(buffer, this->bufferSize_);
    {
        int received = recv(
                client_fd,
                &buffer,
                this->bufferSize_,
                0
        );

        if (received == 0){
            return true;
        }
    }



    serverClient client;

    for (int i = 0; i < this->getNumberOfClients(); ++i) {
        if (this->clients_[i].socket_fd == client_fd) {
            client = this->clients_[i];
        }
    }

    if (client.socket_fd == -1){
        fprintf(stderr, "ERROR, client was lost.\n");
        return false;
    }

    std::string handledBuffer = this->handler_(*this->orm_, buffer);

    if (strcmp(buffer, "EXIT") == 0)
        return handleClientExit(client.socket_fd);

    send(
            client.socket_fd,
            handledBuffer.c_str(),
            strlen(handledBuffer.c_str()),
            0
    );

    return true;
}


bool Server::handleClientExit(int client_fd) {
    std::vector<serverClient>::iterator it;
    for (it = this->clients_.begin(); it != this->clients_.end() ; it++) {
        if (it->socket_fd == client_fd) {
            std::cout<<"closing connection\n";
            close(client_fd);
            FD_CLR(client_fd,&this->clients_fds_);
            this->clients_.erase(it);
            return true;
        }
    }
    return false;
}


int Server::getClientCap() const {
    return this->clientCap_;
}

void Server::setClientCap(int cap) {
    if (cap < 1)
        return;
    this->clientCap_ = cap;
}

int Server::getNumberOfClients() const {
    return this->clients_.size();
}




