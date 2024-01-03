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

    std::cout << "======================================================" << std::endl;
    std::cout << " ---- MENU - Simulacia sirenia poziaru ---- " << std::endl;
    int choice = 0;
    std::cout << "Vyberte moznost:\n"
              << "1. Nahodne vygenerovat svet\n"
              << "2. Nacitat svet z lokalneho suboru\n"
              << "3. Nacitat svet zo servera\n"
              << "Vasa volba: ";
    std::cin >> choice;

    int vyska = 0, sirka = 0;
    if (choice == 1) {
        std::cout << "Zadajte sirku sveta: ";
        std::cin >> sirka;
        std::cout << "Zadajte vysku sveta: ";
        std::cin >> vyska;
    }
    Svet svet = Svet(sirka, vyska);

    if (choice == 2) {
        do {
            std::cout << "Zvolili ste si nacitanie svetu zo suboru!\n";
            std::cout << "Zadajte nazov suboru (bez pripony): \n";
            std::string nazovSub;
            std::cout << ">";
            std::cin >> nazovSub;
            nazovSub += ".txt";
            struct stat buf{};
            if (stat(nazovSub.c_str(), &buf) == -1) {
                std::cout << "Subor neexistuje, zadajte iny subor!! \n";
            } else {
                std::cout << "subor bol uspesne nacitany\n";
                svet.vytvorSvetZoSuboru(nazovSub);
                std::cout << std::endl;
                break;
            }
        } while (true);
    }

    std::cout << "======================================================" << std::endl;
    if (choice == 1) {
        svet.vytvorSvet();
        svet.vytvorPoziarRandomPosition();

    }
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
