#include <telebar/handler/serverHandler/serverGameHandler/serverSetBoardHandler.hpp>

std::string serverSetBoardHandler(ORM &orm, User user, std::string payload) {
    Game game = orm.getById<Game>(user.getGameId());

    // ckecks if the user is in game
    if (game.getId() == -1)
        return "ERROR,user not in game";

    // checks that the user has turn
    if (!game.hasTurn(user.getId()))
        return "WAIT,not your turn";

    if (game.hasEnded()) {
        for(auto& player : game.getPlayers()) {
            orm.save(UserNotification(player.userId, game.getId(), game.getTableName(), std::string(GAME_EVENT_THE_GAME_HAS_ENDED) , game.serialize()));
        }
        orm.remove(game);
        return std::string("END,the game has ended");
    }

    GameAction action (payload);

    if (action.getAction() == GAME_ACTION_PUT_PIECE_TO_THE_RIGHT) {
        if (!game.currentPlayerPutPieceToTheRight(action.getPiece()))
            return "ERROR,cant put piece there";
        orm.update(game);
        return std::string("OK,") + game.serializeForPlayer(user.getId());
    }

    if (action.getAction() == GAME_ACTION_PUT_PIECE_TO_THE_LEFT) {
        if (!game.currentPlayerPutPieceToTheLeft(action.getPiece()))
            return "ERROR,cant put piece there";
        orm.update(game);
        return std::string("OK,") + game.serializeForPlayer(user.getId());
    }

    if (action.getAction() == GAME_ACTION_PASS) {
        if (!game.currentPlayerPass())
            return "ERROR,you cant pass this turn";
        orm.update(game);
        return std::string("OK,") + game.serializeForPlayer(user.getId());
    }


    if (action.getAction() == GAME_ACTION_STEAL_PIECE) {
        if (!game.currentPlayerSteal())
            return "ERROR,you cant steal";
        orm.update(game);
        return std::string("OK,") + game.serializeForPlayer(user.getId());
    }

    return action.serialize();
}
