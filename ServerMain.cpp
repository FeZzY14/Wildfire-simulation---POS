#include <thread>
#include "Server.h"

//
// Created by jmisa on 4. 1. 2024.
//


int main() {
    // Inicializácia servera v samostatnom vlákne
    Server server;
    server.setup(8080);
    auto serverRun = [&server] (  )
    {
        while(!server.getExit()) {
            server.acceptConnection();
            bool check = server.receiveMessage();
            if (!check) {
                break;
            }
        }
    };
    std::thread serverTh( serverRun) ;

    bool pauza = false;

    auto input = [&server, pauza] (  )
    {
        bool pauzaTemp = pauza;
        do {
            if (GetKeyState(VK_END) & 0x8000) {
                if (!pauzaTemp) {
                    pauzaTemp = true;
                    server.closeConnection();
                }
            }
        } while(!pauzaTemp);
    };
    std::thread inputTh( input) ;
    serverTh.join();
    inputTh.join();
}