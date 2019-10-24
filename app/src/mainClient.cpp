#include <iostream>

#include <telebar/kernel/Client.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/configuration/clientConfiguration.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>

int main(int argc, char const *argv[]) {
    Client client(SERVER_IP, SERVER_PORT);
    std::cout<<client.listen()<<"\n";
    std::string message;

    /*message = ",register,{username:diego|password:secret}";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";

    exit(1);*/

    /*message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,startGame,3";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";*/

    /*message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,newMessage,Hello, world";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";*/

    message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,getInfo,";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";

    client.closeConnection();
}
