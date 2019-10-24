#include <telebar/handler/serverHandler/serverGameHandler/serverGetMessageHandler.hpp>

std::string serverNewMessageHandler(ORM &orm, User user, std::string payload) {
    Game game = orm.getById<Game>(user.getGameId());

    if (game.getId() == -1)
        return "ERROR,to send a message you must be in a game";

    Message message(user, payload);
    int messageId = orm.save(message);

    // create the notification
    std::vector<User> gameUsers = orm.getByField<User>("gameId", std::to_string(user.getGameId()));

    for (User& gameUser : gameUsers) {
        orm.save(UserNotification(user.getId(), messageId, message.getTableName(), GAME_EVENT_NEW_MESSAGE));
    }

    return "OK,message created";
}
