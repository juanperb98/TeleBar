#include <telebar/kernel/Server.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/handler/serverHandler/serverHandler.hpp>
#include <iostream>



int main() {
    ORM orm("server.db");

    //orm.createTable<User>();

    //std::cout<<"going to save\n";User user("token", "diego", "secret");orm.save(user);

    std::cout<<"getting all tuples\n";for(auto&useraux:orm.all<User>())std::cout<<useraux.serialize()<<"\n";

    Server server(7707, serverHandlerOrchestrator);while (server.handleNextConnection());

}

