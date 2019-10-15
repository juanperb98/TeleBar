#include <telebar/entity/User.hpp>

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
