#include <iostream>
#include <thread>
#include "PoziarnyBiotop.h"
#include "Bunka.h"
#include "Svet.h"
#include "Server.h"
#include "Client.h"

void startServer(Server *server) {
    server->setup(8080);
    server->acceptConnection();


    std::this_thread::sleep_for(std::chrono::seconds(5));

    server->receiveMessage();
}

int main() {
    // Inicializácia servera v samostatnom vlákne
    Server server;
    std::thread serverThread(startServer, &server);

    // Pockajme na inicializaciu servera
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Client client;
    if (!client.connectToServer("127.0.0.1", 8080)) {
        std::cerr << "Nastala chyba pri inicializácii klienta" << std::endl;
        server.closeConnection();
        return EXIT_FAILURE;
    }

    const char *message = "Ahoj, som klient!";
    if (!client.sendMessage(message)) {
        std::cerr << "Nastala chyba pri komunikácii s serverom" << std::endl;
        server.closeConnection();
        client.closeConnection();
        return EXIT_FAILURE;
    }

    server.closeConnection();
    client.closeConnection();

    serverThread.join();
    return 0;
}
