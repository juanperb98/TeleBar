#include <iostream>

#include <telebar/kernel/Client.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/configuration/clientConfiguration.hpp>

int main(int argc, char const *argv[]) {
    Client client(SERVER_IP, SERVER_PORT);
    std::cout<<client.listen()<<"\n";
    std::string message = "d491e13038cdbf3beec9b7cbb817e,,{username:diego|password:secret}";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";
    client.closeConnection();
}
