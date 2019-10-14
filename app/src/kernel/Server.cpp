#include <telebar/kernel/Server.hpp>

Server::Server(int portno, handlerType requestHandler) {
    this->clientCap = 50;

    this->server.sin_port = htons(portno);
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = htonl(INADDR_ANY);
    this->handler = requestHandler;


    this->server_fd = socket(
            this->server.sin_family,
            SOCK_STREAM,
            0
    );

    {
        int bind_payload = bind(
                this->server_fd,
                (struct sockaddr*) &this->server,
                sizeof(this->server)
        );

        if (bind_payload == -1) {
            fprintf(stderr, "ERROR: unable to bind socket");
            close(this->server_fd);
        }
    }
    {
        int ret_val = listen(this->server_fd, 1);

        if (ret_val == -1) {
            fprintf(stderr, "ERROR, could not set the socket to listen");
            close(this->server_fd);
        }
    }

    FD_ZERO(&this->clients_fd);
    FD_SET(this->server_fd,&this->clients_fd);
    FD_SET(0,&this->clients_fd);
}

bool Server::handdleNextConnection() {

    fd_set clients_fd_aux = this->clients_fd;

    int output;
    output = select(
        FD_SETSIZE,
        &clients_fd_aux,
        NULL,
        NULL,
        NULL
    );

    if (output <= 0) // there is no requests
        return false;

    for (size_t i = 0; i < FD_SETSIZE; ++i) {
        if (FD_ISSET(i, &clients_fd_aux)) {

            // new client is requesting a session
            if (i == this->server_fd)
                return this->createNewSessionForClient();

            // already connected client is sending a payload
            else if (i == 0) {

            }
        }
    }
    return true;
}

bool Server::createNewSessionForClient() {
    serverClient client;

    int client_addr;
    socklen_t client_addr_length = sizeof(client_addr);

    // gets the request
    client.socket_fd = accept(
            this->server_fd,
            (sockaddr*) &client_addr,
            &client_addr_length
    );

    // checks if the fd was created successfully
    if (client.socket_fd == -1)
        return false;

    // checks if the client cap has been reached
    if (this->getNumberOfClients() > this->getClientCap()) {
        this->buffer = "Sorry, but the client cap has been reached, please, try again later.";
        send(
                client.socket_fd,
                this->buffer.c_str(),
                strlen(this->buffer.c_str()),
                0
        );
        close(client.socket_fd);
        return true;
    }

     // lets get that client connected

     // adds the client to the FD set and client list with out the token, as he must log in or create an User to
     // continue
    this->clients.push_back(client);
    FD_SET(client.socket_fd, &clients_fd);

    this->buffer = "Welcome, please, log in or register now.";
    send(
            client.socket_fd,
            this->buffer.c_str(),
            strlen(this->buffer.c_str()),
            0
    );
    return true;
}

int Server::getClientCap() const {
    return this->clientCap;
}

void Server::setClientCap(int cap) {
    if (cap < 1)
        return;
    this->clientCap = cap;
}

int Server::getNumberOfClients() const {
    return this->clients.size();
}
