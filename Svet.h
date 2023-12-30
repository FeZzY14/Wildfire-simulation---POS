//
// Created by spotk on 28. 12. 2023.
//

#ifndef SEMPRACA2_SVET_H
#define SEMPRACA2_SVET_H

#include "Bunka.h"
#include <vector>
#include <cstdlib>
#include "Generator.h"
#include "Vietor.h"
#include <mutex>

class Svet {
private:
    int sirka;
    int vyska;
    Vietor vietor;
    std::vector<std::vector<Bunka>> bunky;
    Generator generator;
    std::mutex mutexPoziar;
    int pocetSimulacii;
public:
    Svet(int sirka, int vyska);
    void vytvorSvet();
    void vytvorPoziarRandomPosition();
    void vypisSvet();
    void spusti();
    void sireniePoziaru();

};


#endif //SEMPRACA2_SVET_H
