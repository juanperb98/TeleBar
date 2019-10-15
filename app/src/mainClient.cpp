#include <iostream>

#include <telebar/kernel/Client.hpp>

int main(int argc, char const *argv[]) {
    Client client("127.0.0.1", 7707);
    client.sendMessage("hello from c1");
    std::cout <<client.listen()<<"\n";
    client.sendMessage("its me again, c1");
    std::cout <<client.listen()<<"\n";
    client.sendMessage("ok bye from c1");
    std::cout <<client.listen()<<"\n";
    client.closeConnection();

    Client client2("127.0.0.1", 7707);
    client2.sendMessage("hello from c2");
    std::cout <<client2.listen()<<"\n";
    client2.sendMessage("its me again, c2");
    std::cout <<client2.listen()<<"\n";
    client2.sendMessage("ok bye from c2");
    std::cout <<client2.listen()<<"\n";
    client2.closeConnection();
}

