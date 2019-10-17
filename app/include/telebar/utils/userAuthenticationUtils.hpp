#ifndef _TELEBAR_UTILS_USERAUTHENTICATIONUTILS_
#define _TELEBAR_UTILS_USERAUTHENTICATIONUTILS_

#include <string>
#include <sstream>
#include <random>

#include <telebar/entity/User.hpp>

namespace utils {
    std::string getUserToken(const User& user);

    unsigned int random_char();

    std::string generate_hex(unsigned int len);

    std::string generateUUID();
}

#endif