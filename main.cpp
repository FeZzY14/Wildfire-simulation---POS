#include <iostream>
#include <thread>
#include "PoziarnyBiotop.h"
#include "Bunka.h"
#include "Svet.h"


int main() {
    Svet svet = Svet(20, 5);
    svet.vytvorSvet();
    svet.vytvorPoziarRandomPosition();
    std::thread vlakno(&Svet::spustiPoziar, &svet);
    std::thread vlakno2(&Svet::spustiRegeneracia, &svet);
    std::thread vlakno3(&Svet::input, &svet);
    vlakno.join();
    vlakno2.join();
    vlakno3.join();
    return 0;
}
