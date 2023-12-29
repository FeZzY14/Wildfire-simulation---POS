//
// Created by spotk on 28. 12. 2023.
//
#include "Svet.h"
#include <vector>
#include <iostream>
#include <thread>

Svet::Svet(int sirka, int vyska) {
    this->sirka = sirka;
    this->vyska = vyska;
    this->vietor = generator.dajVietor();
    this->bunky.resize(this->vyska, std::vector<Bunka>(this->sirka));
}

void Svet::vytvorSvet() {
    for (int i = 0; i < this->vyska; i++) {
        for (int j = 0; j < this->sirka; j++) {
            PoziarBiotop biotop = this->generator.dajNahodnyBiotop();
            bunky[i][j] = Bunka(i, j, biotop);
        }
    }
}

void Svet::vytvorPoziarRandomPosition() {
    int x = this->generator.dajNahodnyPoziar(this->vyska);
    int y = this->generator.dajNahodnyPoziar(this->sirka);
    this->bunky[x][y].setBiotop(PoziarBiotop::Poziar);
}

void Svet::vypisSvet() {
    std::cout << "smer vetra: " << static_cast<char>(this->vietor) << "\n";
    for (int i = 0; i < this->vyska; ++i) {
        for (int j = 0; j < this->sirka; ++j) {
            std::cout << this->bunky[i][j].getZnak() << " ";
        }
        std::cout << std::endl;
    }
}

void Svet::spusti() {
    while(true){
        this->vypisSvet();
    }
}



