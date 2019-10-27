#ifndef _TELEBAR_USERINTERFACE_PRINTERS_MENUPRINTER_
#define _TELEBAR_USERINTERFACE_PRINTERS_MENUPRINTER_

#include <iostream>

#include <telebar/entity/User.hpp>

void clearScreen() {
    std::cout<<"\033[2J\033[1;1H";
}

void printLoginMenu() {
    std::cout<<"welcome to Telebar, please, log in or register to continue\n\n";
    std::cout<<"\t1) log in\n";
    std::cout<<"\t2) register\n";
    std::cout<<"\t0) exit\n";
    std::cout<<"\nPlease, introduce your option: ";
}


void printMainMenu(const User& user) {
    std::cout<<"Hi "<<user.getUsername()<<", ready for a game?\n";
    std::cout<<"\t1) start game\n";
    std::cout<<"\t0) exit\n";
    std::cout<<"\nPlease, introduce your option: ";
}

void printGamePlayingMenu() {
    std::cout<<"\t1) place piece\n";
    std::cout<<"\t2) steal\n";
    std::cout<<"\t3) pass\n";
    std::cout<<"\t0) actualize chat\n";
    std::cout<<"\nPlease, introduce your option: ";
}


#endif