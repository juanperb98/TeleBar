#include <telebar/handler/serverHandler/serverHandler.hpp>
#include <telebar/utils/userAuthenticationUtils.hpp>

#include <string>

std::string serverHandlerOrchestrator(ORM& orm, std::string request) {
    std::string token, action, payload;
    int tokenDelimiter;

    tokenDelimiter = request.find(',');
    token = request.substr(0,tokenDelimiter);
    request = request.substr(tokenDelimiter + 1, request.size());

    tokenDelimiter = request.find(',');
    action = request.substr(0,tokenDelimiter);
    request = request.substr(tokenDelimiter + 1, request.size());

    payload = request;

    // try to authenticate the user
    User user = getUserFromToken(orm, token);
    bool authenticated = user.getId() != -1;

    if (action == "EXIT")
        return request;


    // it the user was not authenticated, try if the user wants to log in ro register
    if (action == SERVER_ACTION_LOGIN && !authenticated)
        return logInHandler(orm, payload);

    else if (action == SERVER_ACTION_REGISTER && !authenticated)
        return registerHandler(orm, payload);
    // if the user didn't register or logged in, return authentication error
    else if (!authenticated)
        return "ERROR, invalid token";

    return serverGameHandlerOrchestrator(orm, user, action, payload);
}


User getUserFromToken(ORM& orm, const std::string& token) {
    if (token.empty())
        return User();
    auto users = orm.getByField<User>("token", token);
    if (users.size() != 1)
        return User();
    else
        return orm.getById<User>(users[0].getId());
}

std::string logInHandler(ORM& orm, const std::string &loginPayload) {
    User user;
    user.deserialize(loginPayload);
    auto users = orm.getByField<User>("username", user.getUsername());
    for (auto& useraux : users){
        if (useraux.getPassword() == user.getPassword())
            return std::string("OK,")+useraux.serialize();
    }
    return "ERROR, invalid user, password convination";
}

std::string registerHandler(ORM& orm, const std::string &loginPayload) {
    User user;
    user.deserialize(loginPayload);
    auto users = orm.getByField<User>("username", user.getUsername());
    if (!users.empty())
        return "ERROR, username already exists";
    user.setToken(utils::generateUUID());
    orm.save(user);
    return std::string("OK,")+user.serialize();
}
