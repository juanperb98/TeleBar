#include <iostream>

#include <telebar/kernel/Client.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/configuration/clientConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/userInterface/printers/gamePrinter.hpp>
#include <telebar/entity/UserNotification.hpp>
#include <telebar/entity/GameAction.hpp>
#include <telebar/userInterface/handlers/clientHandler.hpp>
#include <telebar/userInterface/printers/menuPrinter.hpp>
#include <unistd.h>


int main(int argc, char const *argv[]) {
    // create the connection to the server
    Client client(SERVER_IP, SERVER_PORT);
    std::cout<<client.listen()<<"\n";
    std::string message, retval, payload;
    bool exit = false;
    UserNotification notification;
    Game game;
    GameAction* gameAction;
    std::vector<Message> messages;
    User user;
    int option;

    // clears the pending notifications
    while (handleNotification(client, user, messages));
    messages.clear();


    // start of login

    clearScreen();
    printLoginMenu();
    std::cin>>option;
    if (option == 0)
        handleExit(client);
    else if (option == 1)
        user = logInHandler(client);
    else if (option == 2)
        user = registerHandler(client);

    while (option < 0 || option > 2) {
        clearScreen();
        std::cout<<"out of bounds option, please, try again\n\n";
        printLoginMenu();
        std::cin>>option;
        if (option == 0)
            handleExit(client);
        else if (option == 1)
            user = logInHandler(client);
        else if (option == 2)
            user = registerHandler(client);
    }

    // end of login phase

    while (not exit)
    {
        // check if the user was in a game
        game = getUserGame(client, user);

        // the user is not in a game
        if (game.getId() == -1)
        {
            clearScreen();
            printMainMenu(user);
            std::cin>>option;
            if (option == 0) // exit
                handleExit(client);
            else if (option == 1) // start game
                startGame(client, user);

            while (option < 0 || option > 1) {
                clearScreen();
                std::cout<<"out of bounds option, please, try again\n\n";
                printMainMenu(user);
                std::cin>>option;
                if (option == 0) // exit
                    handleExit(client);
                else if (option == 1) // start game
                    startGame(client, user);
            }
            continue;
        }
        // the user is in a game, playing or searching


        // checks for new messages
        while (handleNotification(client, user, messages));


        // the player is currently playing
        if (game.getNumberOfPlayersCap() == game.getNumberOfPlayers()) {
            // the user has turn
            if (game.hasTurn(user.getId())) {
                handleGameAction(client, game, user, messages);
                continue;
            }
            // the user does not has turn
            else {
                handleGameWaiting(client, game, user, messages);
            }
        }
        // the player is searching for the game
        else {
            clearScreen();
            std::cout<<"press enter to send a message\n";
            std::cout<<"\n\n"<<game.getNumberOfPlayers()<<"/"<<game.getNumberOfPlayersCap()<<" players\n\n";
            std::cout<<"\twaiting for more players to join in";
            std::cout.flush();
            usleep(200000);
            std::cout<<".";
            std::cout.flush();
            usleep(200000);
            std::cout<<".";
            std::cout.flush();
            usleep(200000);
            std::cout<<".";
            std::cout.flush();
            sleep(1);
        }
    }




    handleExit(client);

/*

    // log in

    std::string aux;
    std::cout << "enter your username: ";
    payload = "{username:";
    std::cin >> aux;
    payload += aux;
    std::cout<<"enter your password: ";
    payload += "|password:";
    std::cin>>aux;
    payload += aux;
    payload += "}";
    std::cout<<payload<<"\n";
    message = std::string("null") + "," + SERVER_ACTION_LOGIN + "," + payload;
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";
    user.deserialize(retval.substr(3, retval.size()));
    std::cout<<user.serialize()<<"\n";



    // join the game
    std::cout<<"enter the desired number of players: ";
    std::cin>>payload;

    message = user.getToken() + "," + GAME_ACTION_START_GAME + "," + payload;
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";


    // wait for the game to start
    ready = false;
    while (not ready) {
        message = user.getToken() + "," + GAME_ACTION_GET_UPDATE + "," ;
        client.sendMessage(message);
        retval = client.listen();
        if (retval == "NONE")
            continue;
        std::cout<<retval<<"\n";
        notification.deserialize(retval.substr(3, retval.size()));
        if (notification.getNotification() == GAME_EVENT_THE_GAME_HAS_STARTED)
            ready = true;
    }


    // show the game to the player
    message = user.getToken() + "," + GAME_ACTION_GET_BOARD + ",";
    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";
    game.deserialize(retval.substr(3, retval.size()));
    printGame(game, messages);


    // make a move
    std::cout<<"Enter your piece\n\t Left: ";
    payload = "{L:";
    std::cin>>aux;
    payload += aux;
    std::cout<<"\tRight: ";
    payload += "|R:";
    std::cin>>aux;
    payload += aux;
    payload += "}";
    std::cout<<"where to put it? [L/R]: ";
    std::cin>>aux;

    std::cout<<payload<<"\n";

    if (aux == "L")
        gameAction = new GameAction(GAME_ACTION_PUT_PIECE_TO_THE_LEFT, Piece(payload));
    else
        gameAction = new GameAction(GAME_ACTION_PUT_PIECE_TO_THE_RIGHT, Piece(payload));

    message = user.getToken() + "," + GAME_ACTION_SEND_BOARD_MOVEMENT + "," + gameAction->serialize();

    client.sendMessage(message);
    retval = client.listen();
    std::cout<<retval<<"\n";
    game.deserialize(retval.substr(3, retval.size()));
    printGame(game, messages);


    client.closeConnection();*/
}




