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
    this->pocetSimulacii = 0;
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
    int pocetOpakovani = 0;
    while(pocetOpakovani < 15){
        this->vypisSvet();
        this->sireniePoziaru();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        pocetOpakovani++;
    }
}

void Svet::sireniePoziaru() {
    this->mutexPoziar.lock();
    std::vector<std::vector<Bunka>> tempCopyOfBunky = this->bunky;
    for (int i = 0; i < this->vyska; ++i) {
        for (int j = 0; j < this->sirka; ++j) {
            //najdenie bunky s poziarom
            if (this->bunky[i][j].getBiotop() == PoziarBiotop::Poziar)
            {
                //kontrola ci su bunky vo Von Neumannovom okoli
                for (int distanceI = -1; distanceI <= 1 ; ++distanceI) {
                    for (int distanceJ = -1; distanceJ <= 1; ++distanceJ) {
                        //kontrola ci to nie je v uhlopriecke
                        if (abs(distanceI) + abs(distanceJ) != 1)
                        {
                            //ak sa nenachadza -> preskoc
                            continue;
                        }

                        int neighbourI = i + distanceI;
                        int neighbourJ = j + distanceJ;

                        if (neighbourI >= 0 && neighbourI < this->vyska &&
                            neighbourJ >= 0 && neighbourJ < this->sirka)
                        {
                            //kontrola ci su susedne bunky Les a Luka(Biotopy, ktore budu horiet)
                            if (this->bunky[neighbourI][neighbourJ].getBiotop() == PoziarBiotop::Les ||
                                this->bunky[neighbourI][neighbourJ].getBiotop() == PoziarBiotop::Luka)
                            {
                                double pravdepodonost = 0.0;

                                //kontrola ci je bezvetrie alebo je vietor s nejakym smerom
                                if (this->vietor == Vietor::Bezvetrie)
                                {
                                    pravdepodonost = 0.2;
                                } else if ((this->vietor == Vietor::Hore && distanceI == -1) || (this->vietor == Vietor::Dole && distanceI == 1) ||
                                           (this->vietor == Vietor::Vpravo && distanceJ == 1) || (this->vietor == Vietor::Vlavo && distanceJ == -1))
                                {
                                    pravdepodonost = 0.9;
                                } else {
                                    pravdepodonost = 0.02;
                                }
                                double nahodneCislo = this->generator.dajPravdepodobnost();
                                //std::cout << nahodneCislo << std::endl;
                                if (nahodneCislo <= pravdepodonost) {
                                    std::string nazovBiotopu = "";
                                    if (static_cast<char>(tempCopyOfBunky[neighbourI][neighbourJ].getBiotop()) == 'T')
                                    {
                                        nazovBiotopu = "Les";
                                    }
                                    if (static_cast<char>(tempCopyOfBunky[neighbourI][neighbourJ].getBiotop()) == '.')
                                    {
                                        nazovBiotopu = "Luka";
                                    }

                                    std::cout << "Vypukol poziar v biotope: '" << static_cast<char>(tempCopyOfBunky[neighbourI][neighbourJ].getBiotop())
                                    << "' (" << nazovBiotopu << ") na suradniciach [" << neighbourI << "; " << neighbourJ << "]" <<
                                    " s pravdepodonostou: " << nahodneCislo << "." << std::endl;
                                    tempCopyOfBunky[neighbourI][neighbourJ].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }
                    }
                }

            }
        }
    }

    this->bunky = tempCopyOfBunky;

    this->pocetSimulacii++;
    if (pocetSimulacii >= 3 && this->vietor != Vietor::Bezvetrie) {
        this->vietor = this->generator.dajSmerVetra();
        this->pocetSimulacii = 0;
    }
    this->mutexPoziar.unlock();
}


