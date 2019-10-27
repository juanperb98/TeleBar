#ifndef _TELEBAR_USERINTERFACE_PRINTERS_GAMEPRINTER_
#define _TELEBAR_USERINTERFACE_PRINTERS_GAMEPRINTER_

#include <iostream>

#include <telebar/entity/Game.hpp>
#include <telebar/entity/Message.hpp>

void printBoard(Game game) {
    std::cout<<"board:\n\t";
    auto pieces = game.getInBoardPieces();
    for (int i = 0; i < pieces.size(); ++i) {
        if (i % 10 == 0 && !i == 0)
            std::cout<<"\n\t";
        else if (i != 0)
            std::cout<<" - ";
        std::cout<<pieces[i].getLeft()<<"|"<<pieces[i].getRight();
    }
}

void printPlayers(Game game) {
    auto players = game.getPlayers();

    for (int i = 0; i < players.size(); ++i) {
        if (i != 0)
            std::cout<<"\n\n";

        std::cout<<players[i].username;
        if (players[i].hasTurn)
            std::cout<<" <- turn\n";
        else
            std::cout<<"\n";

        std::cout<<"\t";
        auto pieces = players[i].inHandPieces;
        for (int i = 0; i < pieces.size(); ++i) {
            if (i % 10 == 0 && !i == 0)
                std::cout<<" \n\t";
            else if (i != 0)
                std::cout<<" - ";

            if (pieces[i].isValid())
                std::cout<<pieces[i].getLeft()<<"|"<<pieces[i].getRight();
            else
                std::cout<<"*|*";
        }
    }
    std::cout<<"\n";
}

void printToStealPieces(Game game) {
    std::cout<<"to steal pieces: ";
    if (game.getToStealPieces().empty())
        std::cout<<"none left\n";
    else
     std::cout<<game.getToStealPieces().size()<<" pieces left\n";
}

void printChat(std::vector<Message>& messages) {
    for (auto& message : messages) {
        std::cout<<message.getUsername()<<": "<<message.getMessage()<<"\n";
    }
}

void printGame(Game game) {
    printPlayers(game);
    std::cout<<"\n";
    printToStealPieces(game);
    std::cout<<"\n";
    printBoard(game);
    std::cout<<"\n";
}



void printGameWithMessages(Game& game, std::vector<Message>& messages) {
    printChat(messages);
    std::cout<<"_______________________________________________________________________________";
    std::cout<<"\n‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n";
    printGame(game);
}


#endif