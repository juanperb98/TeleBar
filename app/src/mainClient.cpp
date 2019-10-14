#include <stdio.h>
#include <iostream>
#include <telebar/kernel/Client.hpp>
int main(int argc, char const *argv[]) {
    Client client("127.0.0.1", 7707);
    client.sendMessage("testerino");
    std::cout << client.listen()<< "\n";
}
