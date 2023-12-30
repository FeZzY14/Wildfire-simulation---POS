//
// Created by spotk on 28. 12. 2023.
//

#ifndef SEMPRACA2_SVET_H
#define SEMPRACA2_SVET_H

#include "Bunka.h"
#include <vector>
#include <cstdlib>
#include <thread>
#include <mutex>
#include "Generator.h"
#include "Vietor.h"
#include <mutex>

class Svet {
private:
    int sirka;
    int vyska;
    Vietor vietor;
    int pocetSimulacii;
    std::vector<std::vector<Bunka>> bunky;
    Generator generator;
    std::mutex mutex;
    std::condition_variable stop;
    bool pauza = false;
public:
    Svet(int sirka, int vyska);
    void vytvorSvet();
    void vytvorPoziarRandomPosition();
    void vypisSvet();
    void spustiPoziar();
    void spustiRegeneraciu();
    void sireniePoziaru();
    void regeneraciaBiotopu();
    void inputPause();
    void ulozSvetDoSuboru(const std::string& fileName);
};


#endif //SEMPRACA2_SVET_H
