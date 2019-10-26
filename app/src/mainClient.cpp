#include <iostream>

#include <telebar/kernel/Client.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/configuration/clientConfiguration.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/entity/UserNotification.hpp>
#include <telebar/entity/GameAction.hpp>

int main(int argc, char const *argv[]) {
    Client client(SERVER_IP, SERVER_PORT);
    std::cout<<client.listen()<<"\n";
    std::string message, retval;

    /*message = ",register,{username:pepe|password:secret}";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";

    exit(1);*/

    /*message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,startGame,3";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";

    message = "81ec80cf5f92ca1f89f20e633358e499c75d7bd695eb624e36,startGame,3";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";

    message = "e398ee6b75764e50bf148558b6616d5d8bf373d0963611b91f,startGame,3";
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";*/

    Piece piece(1,3);
    GameAction action(GAME_ACTION_PUT_PIECE_TO_THE_RIGHT, piece);
    message = std::string("dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,setBoard,")+action.serialize();
    client.sendMessage(message);
    std::cout<<client.listen()<<"\n";


    /*message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,getInfo,";
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";*/

    /*UserNotification notification(retval.substr(3, retval.size()));
    message = std::string("dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,getMessage,")+ std::to_string(notification.getRelatedEntityId());
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";

    message = "dffa221b9b26e06cf00f66674c0f28ef276096b23a037e9606,getBoard,";
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";
    */

    client.closeConnection();
}