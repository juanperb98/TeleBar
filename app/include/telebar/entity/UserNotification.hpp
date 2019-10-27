#ifndef _TELEBAR_ENTITY_USERNOTIFICATION_
#define _TELEBAR_ENTITY_USERNOTIFICATION_

#include <telebar/interface/ORMInterface.hpp>

class UserNotification : public ORMInterface {
private:
    std::string notification_;
    std::string relatedEntityName_;
    std::string payload_;
    int relatedEntityId_;
    int relatedUserId_;

public:
    UserNotification(int userId, int entityId, std::string entityName, std::string notification, std::string payload = "");

    UserNotification(std::string stream);

    UserNotification();

    const std::string &getNotification() const;

    const std::string &getRelatedEntityName() const;

    int getRelatedEntityId() const;

    int getRelatedUserId() const;

    std::string serialize() const override;

    const std::string &getPayload() const;

    bool deserialize(std::string stream) override;
};

#endif