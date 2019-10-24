#include <telebar/kernel/Server.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/handler/serverHandler/serverHandler.hpp>
#include <telebar/entity/Game.hpp>
#include <iostream>



int main() {
    ORM orm("server.db");

    orm.createTable<User>();
    orm.createTable<Message>();
    orm.createTable<Game>();
    orm.createTable<UserNotification>();

    Server server(7707, serverHandlerOrchestrator);
    while (server.handleNextConnection());
}

