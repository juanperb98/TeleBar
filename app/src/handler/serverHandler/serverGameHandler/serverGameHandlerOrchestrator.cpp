#include <telebar/handler/serverHandler/serverGameHandler/serverGameHandlerOrchestrator.hpp>


std::string serverGameHandlerOrchestrator(ORM &orm, User user, std::string action, std::string payload) {

    if (action == GAME_ACTION_START_GAME)
        return serverStartGameHandler(orm, user, payload);

    else if (action == GAME_ACTION_GET_UPDATE)
        return payload;

    else if (action == GAME_ACTION_GET_BOARD)
        return serverGetBoardHandler(orm, user, payload);

    else if (action == GAME_ACTION_SEND_BOARD_MOVEMENT)
        return payload;

    else if (action == GAME_ACTION_SEND_MESSAGE)
        return payload;

    else if (action == GAME_ACTION_GET_MESSAGE)
        return payload;

    return "ERROR,Request not found";
}
