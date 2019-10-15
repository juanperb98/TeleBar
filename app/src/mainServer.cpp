#include <telebar/kernel/Server.hpp>
#include <iostream>

std::string handler(std::string test){ std::cout<<test<<"\n"; return test+" | OK";};

int main() {
    Server server(7707, handler);
    server.setClientCap(5);
    while (server.handleNextConnection()){
    }

}

