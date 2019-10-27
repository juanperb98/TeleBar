#include <telebar/entity/UserNotification.hpp>

UserNotification::UserNotification(int userId, int entityId, std::string entityName, std::string notification) {
    this->id_ = -1;
    this->tableName_ = "userNotification";
    this->relatedUserId_ = userId;
    this->relatedEntityId_ = entityId;
    this->relatedEntityName_ = entityName;
    this->notification_ = notification;
}

UserNotification::UserNotification() {
    this->id_ = -1;
    this->tableName_ = "userNotification";
    this->relatedUserId_ = -1;
    this->relatedEntityId_ = -1;
    this->relatedEntityName_ = "";
    this->notification_ = "";
}

UserNotification::UserNotification(std::string stream) {
    this->tableName_ = "userNotification";
    this->deserialize(stream);
}


const std::string &UserNotification::getNotification() const {
    return notification_;
}

const std::string &UserNotification::getRelatedEntityName() const {
    return relatedEntityName_;
}

int UserNotification::getRelatedEntityId() const {
    return relatedEntityId_;
}

int UserNotification::getRelatedUserId() const {
    return relatedUserId_;
}

std::string UserNotification::serialize() const {
    std::string stream;
    stream += "{";

    stream += "id:" + std::to_string(this->getId()) + "|";
    stream += "relatedUserId:" + std::to_string(this->getRelatedUserId()) + "|";
    stream += "relatedEntityId:" + std::to_string(this->getRelatedEntityId()) + "|";
    stream += "relatedEntityName:" + this->getRelatedEntityName() + "|";
    stream += "notification:" + this->getNotification();

    stream += "}";
    return stream;
}

bool UserNotification::deserialize(std::string stream) {
    std::vector<std::tuple<std::string, std::string>> tuples = this->getTuplesFromStream(stream);

    for (auto& tuple : tuples) {
        if (std::get<0>(tuple) == "id")
            this->id_ = atoi(std::get<1>(tuple).c_str());

        else if (std::get<0>(tuple) == "relatedUserId")
            this->relatedUserId_=atoi(std::get<1>(tuple).c_str());

        else if (std::get<0>(tuple) == "relatedEntityId")
            this->relatedEntityId_=atoi(std::get<1>(tuple).c_str());

        else if (std::get<0>(tuple) == "relatedEntityName")
            this->relatedEntityName_=std::get<1>(tuple);

        else if (std::get<0>(tuple) == "notification")
            this->notification_=std::get<1>(tuple);
    }

    return true;
}

