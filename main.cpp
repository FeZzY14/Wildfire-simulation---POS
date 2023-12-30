#include <iostream>
#include <thread>
#include "PoziarnyBiotop.h"
#include "Bunka.h"
#include "Svet.h"


int main() {
    Svet svet = Svet(20, 5);
    svet.vytvorSvet();
    svet.vytvorPoziarRandomPosition();
    std::thread svetThread(&Svet::spusti, &svet);
    svetThread.join();
    return 0;
}
