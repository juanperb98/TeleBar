#include <telebar/kernel/Server.hpp>
#include <iostream>

const std::string& handler(std::string test){ std::cout<<test<<"\n"; return test+" | OK";}

int main() {
    User user("token_diego", "diego", "secret");
    std::cout<<user.serialize()<<"\n";

    User user2(user.serialize());
    std::cout<<user2.serialize()<<"\n";
    std::cout << user2.authenticate() << "\n";
    std::cout<<user2.serialize()<<"\n";

    Server server(7707, handler);
    server.setClientCap(5);
    while (server.handleNextConnection()){
    }

}

