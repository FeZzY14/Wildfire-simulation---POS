#include <iostream>
#include <thread>
#include "Bunka.h"
#include "Svet.h"
#include "Server.h"
#include "Client.h"


int main() {
    // Inicializácia servera v samostatnom vlákne
    Server server;
    server.setup(8080);
    std::thread serverThread(&Server::startServer, std::ref(server), &server);

    Svet svet = Svet(20, 5);
    svet.vytvorSvet();
    svet.vytvorPoziarRandomPosition();
    std::thread threadSvet(&Svet::spustiPoziar, &svet);
    std::thread threadRegeneration(&Svet::spustiRegeneraciu, &svet);
    std::thread threadInputPause(&Svet::inputPause, &svet);
    serverThread.join();
    threadSvet.join();
    threadRegeneration.join();
    threadInputPause.join();

    //server.closeConnection();
    return 0;
}
