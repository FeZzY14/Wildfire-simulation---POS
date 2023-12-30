#include <iostream>
#include <thread>
#include "PoziarnyBiotop.h"
#include "Bunka.h"
#include "Svet.h"


int main() {
    Svet svet = Svet(20, 5);
    svet.vytvorSvet();
    svet.vytvorPoziarRandomPosition();
    std::thread threadSvet(&Svet::spustiPoziar, &svet);
    std::thread threadRegeneration(&Svet::spustiRegeneraciu, &svet);
    std::thread threadInputPause(&Svet::inputPause, &svet);
    threadSvet.join();
    threadRegeneration.join();
    threadInputPause.join();
    return 0;
}
