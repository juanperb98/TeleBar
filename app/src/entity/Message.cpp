#include <telebar/entity/Message.hpp>

Message::Message(User user, std::string message) {
    this->id_ = -1;
    this->tableName_ = "message";
    this->userId_ = user.getId();
    this->username_ = user.getUsername();
    this->gameId_ = user.getGameId();
    this->message_ = message;
}

Message::Message() {
    this->id_ = -1;
    this->tableName_ = "message";
    this->userId_ = -1;
    this->username_ = "";
    this->gameId_ = -1;
    this->message_ = "";
}

Message::Message(std::string stream) {
    this->id_ = -1;
    this->tableName_ = "message";
    this->deserialize(stream);
}

int Message::getUserId() const {
    return userId_;
}

const std::string &Message::getMessage() const {
    return message_;
}

const std::string &Message::getUsername() const {
    return username_;
}

int Message::getGameId() const {
    return gameId_;
}


std::string Message::serialize() const {
    std::string stream;
    stream += "{";
    stream += "id:" + std::to_string(this->getId()) + "|";
    stream += "userId:" + std::to_string(this->getUserId()) + "|";
    stream += "gameId:" + std::to_string(this->getGameId()) + "|";
    stream += "message:[" + this->getMessage() + "]|";
    stream += "username:" + this->getUsername();
    stream += "}";
    return stream;
}

bool Message::deserialize(std::string stream) {
    std::vector<std::tuple<std::string, std::string>> tuples = this->getTuplesFromStream(stream);

    for (auto& tuple : tuples) {
        if (std::get<0>(tuple) == "id")
            this->id_ = atoi(std::get<1>(tuple).c_str());

        else if (std::get<0>(tuple) == "userId")
            this->userId_=atoi(std::get<1>(tuple).c_str());

        else if (std::get<0>(tuple) == "gameId")
            this->gameId_=atoi(std::get<1>(tuple).c_str());

        else if (std::get<0>(tuple) == "message")
            this->message_=std::get<1>(tuple).substr(1, std::get<1>(tuple).size() - 2);

        else if (std::get<0>(tuple) == "username")
            this->username_=std::get<1>(tuple);
    }

    return true;
}



