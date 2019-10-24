#ifndef _TELEBAR_ENTITY_USERNOTIFICATION_
#define _TELEBAR_ENTITY_USERNOTIFICATION_

#include <telebar/interface/ORMInterface.hpp>

class UserNotification : ORMInterface {
private:
    std::string notification_;
    std::string relatedEntityName_;
    int relatedEntityId_;
    int relatedUserId_;

public:
    UserNotification(int userId, int entityId, std::string entityName, std::string notification);

    const std::string &getNotification() const;

    const std::string &getRelatedEntityName() const;

    int getRelatedEntityId() const;

    int getRelatedUserId() const;

    std::string serialize() const override;

    bool deserialize(std::string stream) override;
};

#endif