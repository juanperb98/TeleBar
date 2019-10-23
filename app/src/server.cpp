#include <telebar/kernel/Server.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/handler/serverHandler/serverHandler.hpp>
#include <telebar/entity/Game.hpp>
#include <iostream>



int main() {
    ORM orm("server.db");
    //orm.createTable<User>();


    Game game;

    User user = orm.getById<User>(0);
    User user2 = orm.getById<User>(1);
    //std::cout<<user.serialize()<<"\n";
    //std::cout<<user2.serialize()<<"\n";

    game.addPlayer(user);
    game.addPlayer(user2);
    game.startGame();
    game.debug();
    std::cout<<game.serialize()<<"\n\n";
    Game game2(game.serialize());

    std::cout<<game2.serializeForPlayer(0)<<"\n";

    Server server(7707, serverHandlerOrchestrator);
    //while (server.handleNextConnection());
}

