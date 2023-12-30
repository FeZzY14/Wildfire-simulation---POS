//
// Created by spotk on 28. 12. 2023.
//
#include "Svet.h"
#include <vector>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <string>

Svet::Svet(int sirka, int vyska) {
    this->sirka = sirka;
    this->vyska = vyska;
    this->pocetSimulacii = 0;
    this->pauza = false;
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


void Svet::spustiPoziar() {
    while (true) {
        this->vypisSvet();
        if (pocetSimulacii % 3 == 0 && pocetSimulacii > 0 && this->vietor != Vietor::Bezvetrie) {
            this->vietor = generator.dajSmerVetra();
        }
        std::unique_lock<std::mutex> lock(this->mutex);
        while (pauza) {
            this->stop.wait(lock);
        }
        this->sireniePoziaru();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        pocetSimulacii++;
    }
}

void Svet::spustiRegeneracia() {
    while (true) {
        if (pocetSimulacii > 0) {
            std::unique_lock<std::mutex> lock(this->mutex);
            while (pauza) {
                this->stop.wait(lock);
            }
            this->regeneracia();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}
void Svet::regeneracia() {
    std::vector<std::vector<Bunka>> tempBunky = this->bunky;
    for (int i = 0; i < this->vyska; ++i) {
        for (int j = 0; j < this->sirka; ++j) {
            if (bunky[i][j].getBiotop() == PoziarBiotop::Poziar) {
                double prav = generator.dajPravdepodobnost();
                if (prav <= 0.10) {
                    if (j > 0 && bunky[i][j - 1].getBiotop() == PoziarBiotop::Voda) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                        std::cout << "zmena luka" << prav << " " << i << j << "\n";
                    } else if (j < sirka - 1 && bunky[i][j + 1].getBiotop() == PoziarBiotop::Voda) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                        std::cout << "zmena luka" << prav << " "  << i << j << "\n";
                    } else if (i < vyska - 1 && bunky[i + 1][j].getBiotop() == PoziarBiotop::Voda) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                        std::cout << "zmena luka"  << prav << " " << i << j << "\n";
                    } else if (i > 0 && bunky[i - 1][j].getBiotop() == PoziarBiotop::Voda) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                        std::cout << "zmena luka" << prav << " "  << i << j << "\n";
                    }
                }
            } else if (bunky[i][j].getBiotop() == PoziarBiotop::Luka) {
                if (generator.dajPravdepodobnost() <= 0.02) {
                    if (j > 0 && bunky[i][j - 1].getBiotop() == PoziarBiotop::Les) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                        std::cout << "zmena les" << i << j << "\n";
                    } else if (j < sirka - 1 && bunky[i][j + 1].getBiotop() == PoziarBiotop::Les) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                        std::cout << "zmena les" << i << j << "\n";
                    } else if (i < vyska - 1 && bunky[i + 1][j].getBiotop() == PoziarBiotop::Les) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                        std::cout << "zmena les" << i << j << "\n";
                    } else if (i > 0 && bunky[i - 1][j].getBiotop() == PoziarBiotop::Les) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                        std::cout << "zmena les" << i << j << "\n";
                    }
                }
            }
        }
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
    this->bunky = tempBunky;
}


void Svet::input() {
    while (true) {
        if (GetKeyState('P') & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            if (!pauza) {
                pauza = true;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                do {
                    std::cout << "pauza \npoziar : zalozenie poziaru na suradniciach\n";
                    std::cout << "ulozLok : lokalne ulozenie do suboru\n";
                    std::cout << "ulozServ : ulozenie na server\n";
                    std::cout << "pokracuj : pokracovanie simulacie\n";
                    std::string volba;
                    do {
                        std::cout << ">";
                        std::cin >> volba;
                        if (volba == "poziar") {
                            std::cout << "zvolil si zalozenie\n";
                            std::cout << "zadaj suradnicu x: \n";
                            std::string surX;
                            int surXInt;
                            do {
                                std::cout << ">";
                                std::cin >> surX;
                                try {
                                    surXInt = std::stoi(surX);
                                    if (surXInt < 0 || surXInt > this->sirka - 1) {
                                        throw std::out_of_range("mimo pola");
                                    }
                                    std::cout << "vola: " << surX << "\n";
                                    break;
                                } catch (...) {
                                    std::cout << "zla volba: \n";
                                }
                            } while (true);
                            std::cout << "zadaj suradnicu y: \n";
                            std::string surY;
                            int surYInt;
                            do {
                                std::cout << ">";
                                std::cin >> surY;
                                try {
                                    surYInt = std::stoi(surY);
                                    if (surYInt < 0 || surYInt > this->vyska - 1) {
                                        throw std::out_of_range("mimo pola");
                                    }
                                    std::cout << "vola: " << surY << "\n";
                                    break;
                                } catch (...) {
                                    std::cout << "zla volba \n";
                                }
                            } while (true);
                            this->bunky[surYInt][surXInt].setBiotop(PoziarBiotop::Poziar);
                            break;
                        } else if (volba == "pokracuj") {
                            if (pauza) {
                                std::cout << "SPUSTENE\n";
                                pauza = false;
                                stop.notify_all();
                            }
                            break;
                        } else {
                            std::cout << "zla volba!!\n";
                        }
                    } while (volba != "poziar" || volba != "ulozLok" || volba != "ulozServ");
                } while (pauza);
            }
        }

        if (GetKeyState('A') & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            if (pauza) {
                std::cout << "SPUSTENE\n";
                pauza = false;
                stop.notify_all();
            }
        }
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