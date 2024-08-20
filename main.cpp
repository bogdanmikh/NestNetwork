#include "Server.hpp"
#include "Client.hpp"
#include <iostream>

int main() {
    Nest::Server *server;
    Nest::Client *client;
    std::cout << "1 is server, 2 is client\n";
    int state;
    int port = 7777;
    while (true) {
        std::cin >> state;
        if (state == 1) {
            server = new Nest::Server;
            server->onAttach({"Server", port});
            break;
        } else if (state == 2) {
            client = new Nest::Client;
            std::string ipServer;
            std::cout << "Input ip server: ";
            std::cin >> ipServer;
            client->onAttach({ipServer, port});
            break;
        } else {
            std::cout << "Not Valid value\n";
        }
    }
    for (int i = 0; i < 1000000; ++i) {
        if (state == 1) {
            server->onUpdate();
        } else if (state == 2) {
            client->onUpdate();
//            if (!client->serverValid()) {
//                break;
//            }
        }
    }

    if (state == 1) {
        server->onDetach();
        delete server;
    } else if (state == 2) {
        client->onDetach();
        delete client;
    }
}