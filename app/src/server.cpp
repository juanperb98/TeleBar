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

    User user0 = orm.getById<User>(0);
    User user1 = orm.getById<User>(1);
    User user2 = orm.getById<User>(2);

    /*Game game;
    game.setNumberOfPlayersCap(3);
    User user0 = orm.getById<User>(0);
    User user1 = orm.getById<User>(1);
    User user2 = orm.getById<User>(2);
    game.addPlayer(user0);
    orm.save(game);

    game.addPlayer(user1);
    game.addPlayer(user2);
    orm.save(game);

    std::cout<<game.serialize()<<"\n";

    Game game2(game.serialize());
    std::cout<<game2.serialize()<<"\n";*/




    Server server(7707, serverHandlerOrchestrator);
    while (server.handleNextConnection());
}

