#include <iostream>
#include "PoziarnyBiotop.h"
#include "Bunka.h"
#include "Svet.h"

int main() {
    Svet svet = Svet(10, 10);
    svet.vytvorSvet();
    svet.vytvorPoziarRandomPosition();
    svet.vypisSvet();

    return 0;
}
