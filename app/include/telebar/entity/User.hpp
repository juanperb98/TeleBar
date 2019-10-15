#ifndef _TELEBAR_KERNEL_CLIENT_
#define _TELEBAR_KERNEL_CLIENT_

#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <telebar/utils/SerializableInterface.hpp>

class User{
private:
    std::string username;
    std::string password;
    std::string token;

public:
    User();

    User(std::string token, std::string username = "", std::string password = "");

    const std::string &getUsername() const;

    void setUsername(const std::string &username);

    const std::string &getPassword() const;

    void setPassword(const std::string &password);

    const std::string &getToken() const;

    void setToken(const std::string &token);
};

#endif