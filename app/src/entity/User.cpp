#include <telebar/entity/User.hpp>
#include <telebar/utils/userAuthenticationUtils.hpp>

#include <iostream>

const std::string &User::getUsername() const {
    return username;
}

void User::setUsername(const std::string &username) {
    User::username = username;
}

const std::string &User::getPassword() const {
    return password;
}

void User::setPassword(const std::string &password) {
    User::password = password;
}

const std::string &User::getToken() const {
    return token;
}

void User::setToken(const std::string &token) {
    User::token = token;
}

User::User(std::string stream) {
    this->deserialize(stream);
}

User::User(std::string token, std::string username, std::string password) {
    User::token = token;
    User::username = username;
    User::password = password;
}

User::User() {
    User::token = "";
    User::username = "";
    User::password = "";
}

std::string User::serialize() const {
    return "{token:"+this->token+"|username:"+this->username+"|password:"+this->password+"}";
}

bool User::deserialize(std::string stream) {

    std::vector<std::tuple<std::string, std::string>> tuples = utils::getTouplesFromStream(stream);

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
    return (this->username == user.username) && !this->username.empty() \
        && (this->password == user.password) && !this->password.empty();
}

bool User::operator==(const std::string &token) const {
    return (this->token == token) && !this->token.empty();
}

