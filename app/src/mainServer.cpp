#include <telebar/kernel/Server.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <iostream>

#define _SERVER_

std::string handler(std::string test){ std::cout<<test<<"\n"; return test+" | OK";}

int main() {
    User user("token_diego", "diego", "secret");

    ORM orm;
    orm.createTable(user);
    orm.save(user);

    Server server(7707, handler);
    server.setClientCap(5);
    while (server.handleNextConnection()){
    }

}

