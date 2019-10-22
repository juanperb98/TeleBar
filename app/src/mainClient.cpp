#include <iostream>

#include <telebar/kernel/Client.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/configuration/clientConfiguration.hpp>

int main(int argc, char const *argv[]) {
    Client client(SERVER_IP, SERVER_PORT);
    std::cout<<client.listen()<<"\n";
    std::string message = ",register,{username:juan|password:secret}";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";
    client.closeConnection();
}
