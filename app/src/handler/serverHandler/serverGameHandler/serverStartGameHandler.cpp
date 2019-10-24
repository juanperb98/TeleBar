#include <telebar/handler/serverHandler/serverGameHandler/serverStartGameHandler.hpp>


std::string serverStartGameHandler(ORM &orm, User user, std::string payload) {
    if (user.getGameId() != -1)
        return "ERROR,you are already in a game";

    auto waitingGames = orm.getByField<Game>("isFull", "0");
    int desiredNumberOfPlayers = atoi(payload.c_str());
    if (desiredNumberOfPlayers < MIN_PLAYERS || desiredNumberOfPlayers > MAX_PLAYERS)
        return "ERROR,number of players out of bounds";

    Game selectedGame;

    for (auto& game : waitingGames) {
        if (game.getNumberOfPlayersCap() == desiredNumberOfPlayers) {
            selectedGame = game;
            break;
        }
    }
    if (selectedGame.getId() != -1){ // there is at least one game waiting with the correct number of players
        selectedGame.addPlayer(user);
        // add the gameId to the user
        user.setGameId(selectedGame.getId());
        orm.update(user);
        auto gameUsers = orm.getByField<User>("gameId", std::to_string(selectedGame.getId()));
        for (User gameUser : gameUsers) {
            orm.save(UserNotification(gameUser.getId(), selectedGame.getId(), selectedGame.getTableName(), GAME_EVENT_THE_GAME_HAS_STARTED));
        }
        // if the game is full, start the game
        if (selectedGame.getNumberOfPlayersCap() == selectedGame.getNumberOfPlayers()) {
            selectedGame.startGame();
            orm.update(selectedGame);

            return std::string("OK") + waitingGames[0].serializeForPlayer(user.getId());
        }
        // if not, send the wait to the client
        orm.update(selectedGame);
        return std::string("WAIT") + waitingGames[0].serializeForPlayer(user.getId());
    }
    // there is no game waiting, so create a new one and wait
    selectedGame.setNumberOfPlayersCap(desiredNumberOfPlayers);
    selectedGame.addPlayer(user);
    int gameId = orm.save(selectedGame);
    // add the gameId to the user
    user.setGameId(gameId);
    orm.update(user);
    return std::string("WAIT,") + orm.getById<Game>(gameId).serializeForPlayer(user.getId());
}