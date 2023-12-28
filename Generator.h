//
// Created by jmisa on 28. 12. 2023.
//

#ifndef SEMPRACA2_GENERATOR_H
#define SEMPRACA2_GENERATOR_H


#include <vector>
#include <random>
#include "PoziarnyBiotop.h"

class Generator {
private:
    std::vector<PoziarBiotop> vsetky{PoziarBiotop::Les, PoziarBiotop::Luka, PoziarBiotop::Skala, PoziarBiotop::Voda};
public:
    PoziarBiotop dajNahodnyBiotop() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, vsetky.size() - 1);
        PoziarBiotop biotop = vsetky[dis(gen)];
        return biotop;
    }
};

#endif //SEMPRACA2_GENERATOR_H
