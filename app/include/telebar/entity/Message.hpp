#ifndef _TELEBAR_ENTITY_MESSAGE_
#define _TELEBAR_ENTITY_MESSAGE_

#include <telebar/interface/ORMInterface.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/handler/serverHandler/serverGameHandler/serverGetMessageHandler.hpp>

class Message : public ORMInterface {
private:
    int gameId_;
    int userId_;
    std::string username_;
    std::string message_;


public:
    Message(User user, std::string message);

    explicit Message(std::string stream);

    const std::string &getUsername() const;

    int getGameId() const;

    int getUserId() const;

    const std::string &getMessage() const;

    std::string serialize() const override;

    bool deserialize(std::string stream) override;
};

#endif