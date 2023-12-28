//
// Created by spotk on 28. 12. 2023.
//

#ifndef SEMPRACA2_SVET_H
#define SEMPRACA2_SVET_H

#include "Bunka.h"
#include <vector>
#include <cstdlib>
#include "Generator.h"

class Svet {
private:
    int sirka;
    int vyska;
    std::vector<std::vector<Bunka>> bunky;
    Generator generator;
public:
    Svet(int sirka, int vyska);
    void vytvorSvet();
    void vytvorPoziarRandomPosition();
    void vypisSvet();
};


#endif //SEMPRACA2_SVET_H
