#include <iostream>
#include <unistd.h>
#include <signal.h>


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



Client client(SERVER_IP, SERVER_PORT);


void sigIntHandler(int signum);


int main(int argc, char const *argv[]) {
    signal(SIGINT, sigIntHandler);
    // create the connection to the server
    std::cout<<client.listen()<<"\n";
    std::string message, retval, payload;
    bool exit = false;
    UserNotification notification;
    Game game;
    GameAction* gameAction;
    std::vector<Message> messages;
    User user;
    int option;
    std::string trash;

    // clears the pending notifications
    while (handleNotification(client, user, messages, game));
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

        std::cout<<"game:"<<game.serialize()<<"\n";

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

        handleNotification(client, user, messages, game);
        handleNotification(client, user, messages, game);
        handleNotification(client, user, messages, game);
        handleNotification(client, user, messages, game);
        handleNotification(client, user, messages, game);


        // the player is currently playing
        if (game.getNumberOfPlayersCap() == game.getNumberOfPlayers()) {
            // the user has turn
            if (game.hasTurn(user.getId())) {
                handleGameAction(client, game, user, messages);
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
}




void sigIntHandler (int signum){
    client.closeConnection();
}