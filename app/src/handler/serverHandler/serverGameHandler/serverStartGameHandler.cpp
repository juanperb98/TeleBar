#include <telebar/handler/serverHandler/serverGameHandler/serverStartGameHandler.hpp>


std::string serverStartGameHandler(ORM &orm, User user, std::string payload) {
    {
        Game game = orm.getById<Game>(user.getGameId());
        if (game.getId() != -1)
            return "ERROR,you are already in a game";
    }

    int desiredNumberOfPlayers = atoi(payload.c_str());
    if (desiredNumberOfPlayers < MIN_PLAYERS || desiredNumberOfPlayers > MAX_PLAYERS)
        return "ERROR,number of players out of bounds";

    std::vector<Game> waitingGames;
    waitingGames = orm.getByField<Game>("isFull", "0");



    int selectedGameId = -1;

    for (int i = 0; i < waitingGames.size(); ++i) {
        if (waitingGames[i].getNumberOfPlayersCap() == desiredNumberOfPlayers) {
            selectedGameId = waitingGames[i].getId();
            break;
        }
    }

    // create a new game
    if (selectedGameId == -1) {
        Game game;
        game.setNumberOfPlayersCap(desiredNumberOfPlayers);
        game.addPlayer(user);
        int gameId = orm.save(game);
        user.setGameId(gameId);
        orm.update(user);
        return std::string("WAIT,") + orm.getById<Game>(gameId).serializeForPlayer(user.getId());
    }

    // join the game

    Game game = orm.getById<Game>(selectedGameId);
    game.addPlayer(user);
    user.setGameId(game.getId());
    orm.update(user);
    if (game.getNumberOfPlayersCap() == game.getNumberOfPlayers()) {
        game.startGame();
        for (int& playerId : game.getPlayersIds()) {
            // not needed in the current client
            // orm.save(UserNotification(playerId, game.getId(), game.getTableName(), GAME_EVENT_THE_GAME_HAS_STARTED));
        }
        orm.update(game);
        return std::string("OK,") + game.serializeForPlayer(user.getId());
    }
    orm.update(game);
    return std::string("WAIT,") + game.serializeForPlayer(user.getId());
}