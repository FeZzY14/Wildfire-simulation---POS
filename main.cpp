#include <iostream>
#include <thread>
#include "PoziarnyBiotop.h"
#include "Bunka.h"
#include "Svet.h"

struct nieco {
    int data = 0;
};

int main() {
    Svet svet = Svet(20, 5);
    svet.vytvorSvet();
    svet.vytvorPoziarRandomPosition();
    //svet.spusti();

    nieco data;
    std::thread(svet.spusti, &data);

    return 0;
}
