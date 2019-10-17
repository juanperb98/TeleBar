#include <telebar/entity/User.hpp>
#include <telebar/utils/userAuthenticationUtils.hpp>
#include <telebar/interface/ORMInterface.hpp>
#include <iostream>

const std::string &User::getUsername() const {
    return username_;
}

void User::setUsername(const std::string &username) {
    this->username_ = username;
}

const std::string &User::getPassword() const {
    return password_;
}

void User::setPassword(const std::string &password) {
    this->password_ = password;
}

const std::string &User::getToken() const {
    return token_;
}

void User::setToken(const std::string &token) {
    this->token_ = token;
}

User::User(const std::string& stream) {
    this->deserialize(stream);
}
User::User(const std::string& token, const std::string& username, const std::string& password) {
    User::token_ = token;
    User::username_ = username;
    User::password_ = password;
}

User::User() {
    User::token_ = "";
    User::username_ = "";
    User::password_ = "";
}

std::string User::serialize() const {
    return "{token:" + this->token_ + "|username:" + this->username_ + "|password:" + this->password_ + "}";
}

bool User::deserialize(std::string stream) {

    std::vector<std::tuple<std::string, std::string>> tuples = this->getTuplesFromStream(stream);

    for (auto& tuple : tuples) {
        if (std::get<0>(tuple) == "token")
            this->setToken(std::get<1>(tuple));
        else if (std::get<0>(tuple) == "username")
            this->setUsername(std::get<1>(tuple));
        else if (std::get<0>(tuple) == "password")
            this->setPassword(std::get<1>(tuple));
    }

    return true;
}

bool User::authenticate() {
    this->setToken(utils::getUserToken(*this));
    return !this->getToken().empty();
}

bool User::operator==(const User &user) const {
    return (this->username_ == user.username_) && !this->username_.empty() \
 && (this->password_ == user.password_) && !this->password_.empty();
}

bool User::operator==(const std::string &token) const {
    return (this->token_ == token) && !this->token_.empty();
}

