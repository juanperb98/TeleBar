#include <telebar/handler/serverHandler/serverGameHandler.hpp>


std::string serverGameHandlerOrchestrator(ORM &orm, User user, std::string action, std::string payload) {
    bool inGame = user.getGameId() != -1;
    auto game = orm.getById<User>(user.getGameId());

    if (!inGame && action == "start")
        return serverStartGameHandler(orm, user, payload);

    else if (action == "newMessage")
        return payload;

    else if (inGame && action == "getInfo")
        return "getinfo";

    else if (inGame && action == "getBoard")
        return "getboard";

    else if (inGame && action == "move")
        return payload;

    return "ERROR,Request not found";
}

std::string serverStartGameHandler(ORM &orm, User user, std::string payload) {
    return "WAIT,Waiting for players";
}