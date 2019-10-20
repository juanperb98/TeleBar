#include <telebar/handler/serverHandler/serverHandler.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/utils/userAuthenticationUtils.hpp>

#include <string>

std::string serverHandlerOrchestrator(std::string request) {
    std::string token, action, payload;
    int tokenDelimiter;

    tokenDelimiter = request.find(',');
    token = request.substr(0,tokenDelimiter);
    request = request.substr(tokenDelimiter + 1, request.size());

    tokenDelimiter = request.find(',');
    action = request.substr(0,tokenDelimiter);
    request = request.substr(tokenDelimiter + 1, request.size());

    payload = request;

    User user = authenticationHandler(token);
    bool authenticated = user.getId() != -1;

    if (action == "login" && !authenticated)
        return logInHandler(payload);
    else if (action == "register" && !authenticated)
        return registerHandler(payload);
    else if (!authenticated)
        return "ERROR, invalid token";

    return user.serialize();
}


User authenticationHandler(const std::string& token) {
    if (token.empty())
        return User();
    ORM orm(DB_NAME);
    auto users = orm.getByField<User>("token", token);
    if (users.size() != 1)
        return User();
    else
        return users[0];
}

std::string logInHandler(const std::string &loginPayload) {
    User user;
    user.deserialize(loginPayload);
    ORM orm(DB_NAME);
    auto users = orm.getByField<User>("username", user.getUsername());
    for (auto& useraux : users){
        if (useraux.getPassword() == user.getPassword())
            return std::string("OK,")+useraux.serialize();
    }
    return "ERROR, invalid user, password convination";
}

std::string registerHandler(const std::string &loginPayload) {
    User user;
    user.deserialize(loginPayload);
    ORM orm(DB_NAME);
    auto users = orm.getByField<User>("username", user.getUsername());
    if (users.size() != 0)
        return "ERROR, username already exists";
    user.setToken(utils::generateUUID());
    orm.save(user);
    return std::string("OK,")+user.serialize();
}
