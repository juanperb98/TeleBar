#ifndef _TELEBAR_USERINTERFACE_CLIETNHANDLER_
#define _TELEBAR_USERINTERFACE_CLIETNHANDLER_

User registerHandler(Client &client);

#include <iostream>

#include <telebar/entity/Game.hpp>
#include <telebar/userInterface/printers/menuPrinter.hpp>

void handleExit(Client client) {
    client.closeConnection();
    std::cout<<"ok bye!\n";
    exit(0);
}


User registerHandler(Client &client) {
    User user;
    std::string aux, payload, message, retval, statusCode;
    std::cout << "enter the desired username: ";
    payload = "{username:";
    std::cin >> aux;
    payload += aux;
    std::cout<<"enter your new password: ";
    payload += "|password:";
    std::cin>>aux;
    payload += aux;
    payload += "}";
    message = std::string("null") + "," + SERVER_ACTION_REGISTER + "," + payload;
    client.sendMessage(message);
    retval = client.listen();
    statusCode = retval.substr(0,retval.find(','));
    while (statusCode != "OK") {
        std::cout << "the username already exists, please, try another one\n";
        std::cout << "\nenter the desired username: ";
        payload = "{username:";
        std::cin >> aux;
        payload += aux;
        std::cout<<"enter your new password: ";
        payload += "|password:";
        std::cin>>aux;
        payload += aux;
        payload += "}";
        message = std::string("null") + "," + SERVER_ACTION_REGISTER + "," + payload;
        client.sendMessage(message);
        retval = client.listen();
        statusCode = retval.substr(0,retval.find(','));
    }

    user.deserialize(retval.substr(retval.find(',') + 1, retval.size()));
    return user;
}


User logInHandler(Client& client) {
    User user;
    std::string aux, payload, message, retval, statusCode;
    std::cout << "enter your username: ";
    payload = "{username:";
    std::cin >> aux;
    payload += aux;
    std::cout<<"enter your password: ";
    payload += "|password:";
    std::cin>>aux;
    payload += aux;
    payload += "}";
    message = std::string("null") + "," + SERVER_ACTION_LOGIN + "," + payload;
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";
    statusCode = retval.substr(0,retval.find(','));
    while (statusCode != "OK") {
        std::cout << "username or password wasn't correct, do you want to register? [y/n]\n";
        std::cin>>aux;
        if (aux == "y") {
            return registerHandler(client);
        }
        std::cout << "\nenter your username: ";
        payload = "{username:";
        std::cin >> aux;
        payload += aux;
        std::cout<<"enter your password: ";
        payload += "|password:";
        std::cin>>aux;
        payload += aux;
        payload += "}";
        message = user.getToken() + "," + SERVER_ACTION_LOGIN + "," + payload;
        client.sendMessage(message);
        retval = client.listen();
        statusCode = retval.substr(0,retval.find(','));
    }

    user.deserialize(retval.substr(retval.find(',') + 1, retval.size()));
    return user;
}


Game getUserGame(Client& client, User& user) {
    Game game;

    std::string message = user.getToken() + "," + GAME_ACTION_GET_BOARD + ",";
    client.sendMessage(message);
    std::string retval = client.listen();
    std::string statusCode = retval.substr(0,retval.find(','));

    if (statusCode == "OK")
        game.deserialize(retval.substr(retval.find(',') + 1 ,retval.size()));

    if (statusCode == "WAIT")
        game.deserialize(retval.substr(retval.find(',') + 1 ,retval.size()));

    return game;
}


void startGame(Client& client, User& user) {
    int numberOfPlayers;
    std::cout<<"enter the desired number of players: ";
    std::cin>>numberOfPlayers;
    std::string message = user.getToken() + "," + GAME_ACTION_START_GAME + "," + std::to_string(numberOfPlayers);
    client.sendMessage(message);
    std::string retval = client.listen();
    std::string statusCode = retval.substr(0,retval.find(','));
    std::string error = retval.substr(retval.find(',')+1,retval.size());
    if (statusCode != "OK")
        fprintf(stderr, "%s", error.c_str());
}


std::string handleGameActionPlace(Client& client, Game& game, User& user, std::vector<Message>& messages) {
    std::string statusCode, retval, message, payload, aux;

    clearScreen();
    printGame(game);
    std::cout << "\nEnter your piece\n\t Left: ";
    payload = "{L:";
    std::cin >> aux;
    payload += aux;
    std::cout << "\tRight: ";
    payload += "|R:";
    std::cin >> aux;
    payload += aux;
    payload += "}";
    Piece piece(payload);


    while (!game.currentPlayerHasPiece(piece)) {
        clearScreen();
        printGame(game);
        std::cout << "\nyou dont have that piece, please, select one of your pieces\n";
        std::cout << "\nEnter your piece\n\t Left: ";
        payload = "{L:";
        std::cin >> aux;
        payload += aux;
        std::cout << "\tRight: ";
        payload += "|R:";
        std::cin >> aux;
        payload += aux;
        payload += "}";
        piece.deserialize(payload);
    }

    std::cout << "where to put it? [L/R]: ";
    std::cin >> aux;

    GameAction *gameAction;
    if (aux == "L")
        gameAction = new GameAction(GAME_ACTION_PUT_PIECE_TO_THE_LEFT, piece);
    else
        gameAction = new GameAction(GAME_ACTION_PUT_PIECE_TO_THE_RIGHT, piece);

    message = user.getToken() + "," + GAME_ACTION_SEND_BOARD_MOVEMENT + "," + gameAction->serialize();
    client.sendMessage(message);
    retval = client.listen();
    return retval;
}


std::string handleGameActionSteal(Client& client, Game& game, User& user, std::vector<Message>& messages) {
    std::string statusCode, retval, message, payload, aux;
    GameAction gameAction(GAME_ACTION_STEAL_PIECE, Piece(-1, -1));
    message = user.getToken() + "," + GAME_ACTION_SEND_BOARD_MOVEMENT + "," + gameAction.serialize();
    client.sendMessage(message);
    retval = client.listen();
    return retval;
}


std::string handleGameActionPass(Client& client, Game& game, User& user, std::vector<Message>& messages) {
    std::string statusCode, retval, message, payload, aux;
    GameAction gameAction(GAME_ACTION_PASS, Piece(-1, -1));
    message = user.getToken() + "," + GAME_ACTION_SEND_BOARD_MOVEMENT + "," + gameAction.serialize();
    client.sendMessage(message);
    retval = client.listen();
    return retval;
}

void handleGameAction(Client& client, Game& game, User& user, std::vector<Message>& messages) {
    int option;
    std::string retval;
    std::string statusCode;
    std::string error;

    while (statusCode != "OK")
    {
        clearScreen();
        printGameWithMessages(game, messages);
        if (!error.empty())
            std::cout<<error<<"\n";
        std::cout<<"\n";
        printGamePlayingMenu();
        std::cin >> option;
        if (option == 0) { // reload
            statusCode = "OK";
            continue;
        }
        else if (option == 1) { // reload
            retval = handleGameActionPlace(client, game, user, messages);
        } else if (option == 2) { // steal
            retval = handleGameActionSteal(client, game, user, messages);
        } else if (option == 3) { // pass
            retval = handleGameActionPass(client, game, user, messages);
        }

        while (option < 0 || option > 3) {
            clearScreen();
            printGameWithMessages(game, messages);
            std::cout << "out of bounds option, please, try again\n\n";
            printGamePlayingMenu();
            std::cin >> option;
            if (option == 0) { // reload
                statusCode = "OK";
                continue;
            }
            else if (option == 1) { // reload
                retval = handleGameActionPlace(client, game, user, messages);
            } else if (option == 2) { // steal
                retval = handleGameActionSteal(client, game, user, messages);
            } else if (option == 3) { // pass
                retval = handleGameActionPass(client, game, user, messages);
            }
        }
        statusCode = retval.substr(0,retval.find(','));
        error =  retval.substr(retval.find(',') + 1, retval.size());
    }
}

void handleGameWaiting(Client& client, Game& game, User& user, std::vector<Message>& messages) {
    clearScreen();
    printGameWithMessages(game, messages);
    std::cout<<"\n";
    std::cout<<"waiting for your turn press enter to actualize, or write in the char\n\nChat: ";
    std::string input;
    getline(std::cin, input);
    if (input == ""){
        return;
    }

    std::string payload = user.getToken() + "," + GAME_ACTION_SEND_MESSAGE + "," + input;
    client.sendMessage(payload);
    std::string retval = client.listen();
    std::string statusCode = retval.substr(0,retval.find(','));
    std::string error =  retval.substr(retval.find(',') + 1, retval.size());
    if (statusCode != "OK" && statusCode != "END")
        fprintf(stderr, "%s", error.c_str());
}



void handleMessage(Client& client, User& user, std::vector<Message>& messages, int messageId) {
    std::string payload = user.getToken() + "," + GAME_ACTION_GET_MESSAGE + "," + std::to_string(messageId);
    client.sendMessage(payload);
    std::string retval = client.listen();
    std::string statusCode = retval.substr(0,retval.find(','));
    std::string data =  retval.substr(retval.find(',') + 1, retval.size());
    if (statusCode != "OK") {
        fprintf(stderr, "%s", data.c_str());
        return;
    }
    Message message(data);
    std::cout<<"message:"<<message.getMessage()<<"\n";
    std::cout<<"serialized message:"<<message.serialize()<<"\n";
    if (!message.getMessage().empty())
        messages.emplace_back(message);
}


void handleGameEnd(Client& client, User& user, Game& game) {
    printGame(game);
    std::cout<<"\nThe winner is "<<game.getWinningPlayer().username<<"!";
    std::cout<<"\n\npress enter to continue.\n";
    user.setGameId(-1);
    std::string input;
    getline(std::cin, input);
}


bool handleNotification(Client& client, User& user, std::vector<Message>& messages, Game& game) {
    std::string payload = user.getToken() + "," + GAME_ACTION_GET_UPDATE + "," + std::to_string(user.getId());
    client.sendMessage(payload);
    std::string retval = client.listen();
    std::string statusCode = retval.substr(0,retval.find(','));
    std::string data =  retval.substr(retval.find(',') + 1, retval.size());

    UserNotification notification(data);

    std::cout<<notification.serialize()<<"\n";

    if (notification.getNotification() == GAME_EVENT_NEW_MESSAGE) {
        handleMessage(client, user, messages, notification.getRelatedEntityId());
        return true;
    }
    else if (notification.getNotification() == GAME_EVENT_THE_GAME_HAS_ENDED) {
        handleGameEnd(client, user, game);
        return true;
    }
    return false;

}




#endif