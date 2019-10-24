#include <telebar/handler/serverHandler/serverGameHandler/serverGetMessageHandler.hpp>

std::string serverGetMessageHandler(ORM &orm, User user, std::string payload) {
    // TODO: add security here (users shall be in same game)
    int messageId = atoi(payload.c_str());
    Message message = orm.getById<Message>(messageId);
    if (message.getId() == -1)
        return "ERROR,message not found";

    return std::string("OK") + message.serialize();
}
