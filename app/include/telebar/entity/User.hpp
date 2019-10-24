#ifndef _TELEBAR_ENTITY_CLIENT_
#define _TELEBAR_ENTITY_CLIENT_

#include <string>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <telebar/utils/ORM.hpp>

/**
 * user entity class, it holds the information of an user, it can be serialized and stored in the database.
 */
class User : public ORMInterface {
private:
    std::string username_;
    std::string password_;
    std::string token_;
    int gameId_;

public:
    User();

    explicit User(const std::string& stream);

    User(const std::string& token, const std::string& username, const std::string& password);

    const int getUsername() const;

    void setUsername(const std::string &username);

    const std::string &getPassword() const;

    void setPassword(const std::string &password);

    const std::string &getToken() const;

    void setToken(const std::string &token);

    std::string serialize() const override;

    bool deserialize(std::string stream) override;

    bool authenticate();

    bool operator==(const User& user) const;

    bool operator==(const std::string& token) const;

    int getGameId();

    int setGameId(int id);
};

#endif