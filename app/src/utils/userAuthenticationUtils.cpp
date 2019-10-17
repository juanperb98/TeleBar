#include <telebar/utils/userAuthenticationUtils.hpp>

std::string utils::getUserToken(const User& user) {
    // TODO: finish db controller
    //auto raw_users_db = "select * from telebar.users where telebar.users.name == user.name and telebar.users.password == user.password";
    std::vector<User> users_db = {
            User("token_juanpe", "juanpe", "password"),
            User("token_diego", "diego", "secret")
    };

    for (User& user_db : users_db) {
        if (user==user_db)
            return utils::generateUUID();
    }

    return std::__cxx11::string();
}

unsigned int utils::random_char() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

std::string utils::generate_hex(const unsigned int len) {
    std::stringstream ss;
    for (auto i = 0; i < len; i++) {
        const auto rc = random_char();
        std::stringstream hexstream;
        hexstream << std::hex << rc;
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}

std::string utils::generateUUID() {
    return utils::generate_hex(25);
}


