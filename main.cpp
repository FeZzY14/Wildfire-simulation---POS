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
    // Vytvorenie menu
    std::cout << "======================================================" << std::endl;
    std::cout << " ---- MENU - Simulacia sirenia poziaru ---- " << std::endl;
    int choice = 0;
    std::cout << "Vyberte moznost:\n"
              << "1. Nahodne vygenerovat svet cez server\n"
              << "2. Nacitat svet z lokalneho suboru\n"
              << "3. Nacitat svet zo servera\n"
              << "Vasa volba: ";
    std::cin >> choice;

    int vyska = 0, sirka = 0;
    Server server;
    std::thread* serverThread = nullptr;
    Client client;

    if (choice == 1) {
        serverThread = new std::thread(startServer, &server);

        // Pockajme na inicializaciu servera
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (!client.connectToServer("127.0.0.1", 8080)) {
            std::cerr << "Nastala chyba pri inicializacii klienta" << std::endl;
            server.closeConnection();
            return EXIT_FAILURE;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Zadajte sirku sveta: ";
        std::cin >> sirka;
        std::cout << "Zadajte vysku sveta: ";
        std::cin >> vyska;
    }
    Svet svet = Svet(sirka, vyska);

    if (choice == 2) {
        std::string nazovSuboru;
        std::cout << "Zadajte nazov suboru: ";
        std::cin >> nazovSuboru;
        nazovSuboru += ".txt";
        if (svet.vytvorSvetSoSuboru(nazovSuboru) == -1) {
            std::cerr << "Nastala chyba pri nacitani sveta z suboru" << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cout << "======================================================" << std::endl;
    if (choice == 1) {
        svet.vytvorSvet();
        svet.vytvorPoziarRandomPosition();
        const char *worldChars = svet.svetToChars();
        if (!client.sendMessage(worldChars)) {
            std::cerr << "Nastala chyba pri komunikacii s serverom" << std::endl;
            server.closeConnection();
            client.closeConnection();
            delete[] worldChars;
            return EXIT_FAILURE;
        }

        delete[] worldChars;

        server.closeConnection();
        client.closeConnection();

        serverThread->join();
        delete serverThread;
    }

    std::thread threadSvet(&Svet::spustiPoziar, &svet);
    std::thread threadRegeneration(&Svet::spustiRegeneraciu, &svet);
    std::thread threadInputPause(&Svet::inputPause, &svet);
    threadSvet.join();
    threadRegeneration.join();
    threadInputPause.join();
    return 0;
}
