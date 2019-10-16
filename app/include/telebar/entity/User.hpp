#ifndef _TELEBAR_ENTITY_CLIENT_
#define _TELEBAR_ENTITY_CLIENT_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <telebar/utils/SerializableInterface.hpp>
#include <telebar/utils/serializableUtils.hpp>

class User : public SerializableInterface{
private:
    std::string username;
    std::string password;
    std::string token;

public:
    User();

    User(std::string stream);

    User(std::string token, std::string username, std::string password);

    const std::string &getUsername() const;

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
};

#endif