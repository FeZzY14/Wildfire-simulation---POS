//
// Created by jmisa on 28. 12. 2023.
//

#ifndef SEMPRACA2_GENERATOR_H
#define SEMPRACA2_GENERATOR_H


#include <vector>
#include <random>
#include "PoziarnyBiotop.h"
#include "Vietor.h"

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

    int dajNahodnyPoziar(int rozmer) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, rozmer - 1);
        int nahoda = dis(gen);
        return nahoda;

    }

    double dajNahodneCislo() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);
        double nahoda = dis(gen);
        return nahoda;
    }

    Vietor dajVietor() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0,1);
        double nahoda = dis(gen);

        if (nahoda <= 0.9) {
            return Vietor::Bezvetrie;
        } else {
            std::vector<Vietor> vetry{Vietor::Dole, Vietor::Hore, Vietor::Vlavo, Vietor::Vpravo};
            std::random_device rd2;
            std::mt19937 gen2(rd2());
            std::uniform_int_distribution<size_t> dis2(0,vetry.size() - 1);

            return vetry[dis2(gen2)];
        }
    }

    Vietor dajSmerVetra() {
        std::vector<Vietor> vetry{Vietor::Dole, Vietor::Hore, Vietor::Vlavo, Vietor::Vpravo};
        std::random_device rd2;
        std::mt19937 gen2(rd2());
        std::uniform_int_distribution<size_t> dis2(0, vetry.size() - 1);

        return vetry[dis2(gen2)];
    }
};

#endif //SEMPRACA2_GENERATOR_H
