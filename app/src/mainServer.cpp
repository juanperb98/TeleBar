#include <telebar/kernel/Server.hpp>
#include <iostream>

std::string handler(std::string test){ std::cout<<test<<"\n"; return "OK";};

int main() {
    Server server(7707, handler);
    while (true){
        server.handdleNextConnection();
        sleep(1);
    }

}