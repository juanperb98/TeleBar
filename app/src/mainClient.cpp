#include <iostream>

#include <telebar/kernel/Client.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/configuration/clientConfiguration.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/entity/UserNotification.hpp>

int main(int argc, char const *argv[]) {
    Client client(SERVER_IP, SERVER_PORT);
    std::cout<<client.listen()<<"\n";
    std::string message, retval;

    /*message = ",register,{username:diego|password:secret}";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";

    exit(1);*/

    /*message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,startGame,3";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";*/

    message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,newMessage,Hello, world, this is the message of the gods this is too god to be true";
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";

    message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,getInfo,";
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";

    UserNotification notification(retval.substr(3, retval.size()));
    message = std::string("dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,getMessage,")+ std::to_string(notification.getRelatedEntityId());
    //std::cout<<message<<"\n";
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";

    message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,getBoard,";
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";

    client.closeConnection();
}
