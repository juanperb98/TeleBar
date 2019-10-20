#include <iostream>

#include <telebar/kernel/Client.hpp>
#include <telebar/entity/User.hpp>

int main(int argc, char const *argv[]) {
    Client client("127.0.0.1", 7707);
    std::cout<<client.listen()<<"\n";
    std::string message = "asd,action,payload";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";
    client.closeConnection();
}

