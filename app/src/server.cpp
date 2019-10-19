#include <telebar/kernel/Server.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <iostream>


std::string handler(std::string test){ std::cout<<test<<"\n"; return test+" | OK";}

int main() {
    ORM orm("server.db");
    User user("token_diego", "diego", "secret");
    orm.createTable<User>();
    std::cout<<"going to save\n";
    orm.save(user);
    std::cout<<"getting all tuples\n";
    auto users = orm.all<User>();
    for (auto& useraux : users ) {
        std::cout<<useraux.serialize()<<"\n";
    }

    //Server server(7707, handler);
    //server.setClientCap(5);
    //while (server.handleNextConnection()) {
    //}

}

