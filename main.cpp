#include <iostream>
#include <thread>
#include "Bunka.h"
#include "Svet.h"
#include "Server.h"
#include "Client.h"


int main() {
    int choice = 0;
    do {
        std::cout << "======================================================" << std::endl;
        std::cout << " ---- MENU - Simulacia sirenia poziaru ---- " << std::endl;
        std::cout << "Vyberte moznost:\n"
                  << "1. Nahodne vygenerovat svet\n"
                  << "2. Nacitat svet z lokalneho suboru\n"
                  << "3. Nacitat svet zo servera\n"
                  << "Vasa volba: ";
        std::cin >> choice;
    } while(choice < 1 || choice > 3);
    int vyska = 0, sirka = 0;
    if (choice == 1) {
        do {
            std::cout << "Zadajte sirku sveta: ";
            std::cin >> sirka;
        } while(sirka < 4 || sirka > 30);
        do {
            std::cout << "Zadajte vysku sveta: ";
            std::cin >> vyska;
        } while(sirka < 4 || sirka > 30);
    }
    Svet svet = Svet(sirka, vyska);
    if (choice == 2) {
        do {
            std::cout << "Zvolili ste si nacitanie svetu zo suboru!\n";
            std::cout << "Zadajte nazov suboru (bez pripony): \n";
            std::string nazovSub;
            std::cout << ">";
            std::cin >> nazovSub;
            if (nazovSub == "worldsServer") {
                std::cout << "Nazov 'worldsServer' nie je povoleny. Skuste iny nazov.\n";
                continue;
            }
            nazovSub += ".txt";
            struct stat buf{};
            if (stat(nazovSub.c_str(), &buf) == -1) {
                std::cout << "Subor neexistuje, zadajte iny subor!! \n";
            } else {
                std::cout << "Subor bol uspesne nacitany\n";
                svet.vytvorSvetZoSuboru(nazovSub);
                std::cout << std::endl;
                break;
            }
        } while (true);
    } else if (choice == 3) {
        svet.nacitanieSvetuZoServera();
    }

    std::cout << "======================================================" << std::endl;
    if (choice == 1) {
        svet.vytvorSvet();
        svet.vytvorPoziarRandomPosition();

    }
    std::thread threadSvet(&Svet::spustiPoziar, &svet);
    std::thread threadRegeneration(&Svet::spustiRegeneraciu, &svet);
    std::thread threadInputPause(&Svet::inputPause, &svet);
    threadSvet.join();
    threadRegeneration.join();
    threadInputPause.join();
    return 0;
}
