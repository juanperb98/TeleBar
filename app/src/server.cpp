#include <telebar/kernel/Server.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/handler/serverHandler/serverHandler.hpp>
#include <iostream>



int main() {
    ORM orm("server.db");
    Server server(7707, serverHandlerOrchestrator);
    while (server.handleNextConnection());
}

