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

    int on=1;
    setsockopt( this->server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    {
        int bind_payload = bind(
                this->server_fd,
                (struct sockaddr*) &this->server,
                sizeof(this->server)
        );

        if (bind_payload == -1) {
            fprintf(stderr, "ERROR: unable to bind socket.\n");
            close(this->server_fd);
            exit(1);
        }
    }
    {
        int ret_val = listen(this->server_fd, 10);

        if (ret_val == -1) {
            fprintf(stderr, "ERROR, could not set the socket to listen.\n");
            close(this->server_fd);
            exit(2);
        }
    }

    FD_ZERO(&this->clients_fds);
    FD_SET(this->server_fd,&this->clients_fds);
    FD_SET(0,&this->clients_fds);
    FD_CLR(STDIN_FILENO, &this->clients_fds);
}

bool Server::handleNextConnection() {
    fd_set clients_fds_aux = this->clients_fds;

    int output;
    output = select(
        FD_SETSIZE,
        &clients_fds_aux,
        NULL,
        NULL,
        NULL
    );

    if (output <= 0) // there is no requests
        return false;


    for (size_t i = 0; i < FD_SETSIZE; ++i) {
        if (FD_ISSET(i, &clients_fds_aux)) {

            // new client is requesting a session
            if (i == this->server_fd)
                return this->createNewSessionForClient();

            // already connected client is sending a payload
            else
                return this->handleConnectionFromClient(i);
        }
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
    FD_SET(client.socket_fd, &clients_fds);

    this->buffer = "Welcome, please, log in or register now.";
    /*send(
            client.socket_fd,
            this->buffer.c_str(),
            strlen(this->buffer.c_str()),
            0
    );*/
    return true;
}

bool Server::handleConnectionFromClient(int client_fd) {
    int socket_fd = -1;
    int client_addr;
    socklen_t client_addr_length = sizeof(client_addr);

    char buffer_c[255];
    bzero(buffer_c, 255);
    {
        int received = recv(
                client_fd,
                &buffer_c,
                255,
                0
        );

        if (received == 0){
            return true;
        }
    }



    serverClient client;

    for (int i = 0; i < this->getNumberOfClients(); ++i) {
        if (this->clients[i].socket_fd == client_fd) {
            client = this->clients[i];
        }
    }

    if (client.socket_fd == -1){
        fprintf(stderr, "ERROR, client was lost.\n");
        return false;
    }

    this->buffer = this->handler(buffer_c);

    if (strcmp(buffer_c, "EXIT") == 0)
        return handleClientExit(client.socket_fd);

    send(
            client.socket_fd,
            this->buffer.c_str(),
            strlen(this->buffer.c_str()),
            0
    );

    return true;
}


bool Server::handleClientExit(int client_fd) {
    std::vector<serverClient>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end() ; it++) {
        if (it->socket_fd == client_fd) {
            std::cout<<"closing connection\n";
            close(client_fd);
            FD_CLR(client_fd,&this->clients_fds);
            this->clients.erase(it);
            return true;
        }
    }
    return false;
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




