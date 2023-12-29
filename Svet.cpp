//
// Created by spotk on 28. 12. 2023.
//
#include "Svet.h"
#include <vector>
#include <iostream>
#include <thread>
#include <Windows.h>

Svet::Svet(int sirka, int vyska) {
    this->sirka = sirka;
    this->vyska = vyska;
    this->kolo = 0;
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
    while (true) {
        this->vypisSvet();
        if (kolo % 3 == 0 && kolo > 0 && this->vietor != Vietor::Bezvetrie) {
            this->vietor = generator.dajSmerVetra();
        }
        std::unique_lock<std::mutex> lock(this->mutex);
        while (pauza) {
            this->stop.wait(lock);
        }
        this->sireniePoziaru();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        kolo++;
    }
}

void Svet::spusti2() {
    while (true) {
        std::unique_lock<std::mutex> lock(this->mutex);
        while (pauza) {
            this->stop.wait(lock);
        }
        this->regeneracia();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Svet::sireniePoziaru() {
    std::vector<std::vector<Bunka>> tempBunky = this->bunky;
    for (int i = 0; i < this->vyska; ++i) {
        for (int j = 0; j < this->sirka; ++j) {
            if (bunky[i][j].getBiotop() == PoziarBiotop::Poziar) {
                switch (this->vietor) {
                    case Vietor::Vlavo :
                        if (generator.dajPravdepodobnost() <= 0.90) {
                            if (j > 0) {
                                if (bunky[i][j - 1].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i][j - 1].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i][j - 1].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }

                        if (generator.dajPravdepodobnost() <= 0.02) {
                            if (j < sirka - 1) {
                                if (bunky[i][j + 1].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i][j + 1].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i][j + 1].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }
                        break;
                    case Vietor::Bezvetrie:
                        if (generator.dajPravdepodobnost() <= 0.20) {
                            if (j > 0) {
                                if (bunky[i][j - 1].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i][j - 1].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i][j - 1].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                            if (j < sirka - 1) {
                                if (bunky[i][j + 1].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i][j + 1].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i][j + 1].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                            if (i < vyska - 1) {
                                if (bunky[i + 1][j].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i + 1][j].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i + 1][j].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                            if (i > 0) {
                                if (bunky[i - 1][j].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i - 1][j].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i - 1][j].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }
                        break;
                    case Vietor::Hore:
                        if (generator.dajPravdepodobnost() <= 0.90) {
                            if (i > 0) {
                                if (bunky[i - 1][j].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i - 1][j].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i - 1][j].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }

                        if (generator.dajPravdepodobnost() <= 0.02) {
                            if (i < vyska - 1) {
                                if (bunky[i + 1][j].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i + 1][j].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i + 1][j].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }
                        break;
                    case Vietor::Dole:
                        if (generator.dajPravdepodobnost() <= 0.90) {
                            if (i < vyska - 1) {
                                if (bunky[i + 1][j].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i + 1][j].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i + 1][j].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }

                        if (generator.dajPravdepodobnost() <= 0.02) {
                            if (i > 0) {
                                if (bunky[i - 1][j].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i - 1][j].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i - 1][j].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }
                        break;
                    case Vietor::Vpravo:
                        if (generator.dajPravdepodobnost() <= 0.90) {
                            if (j < sirka - 1) {
                                if (bunky[i][j + 1].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i][j + 1].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i][j + 1].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }

                        if (generator.dajPravdepodobnost() <= 0.02) {
                            if (j > 0) {
                                if (bunky[i][j - 1].getBiotop() == PoziarBiotop::Luka ||
                                    bunky[i][j - 1].getBiotop() == PoziarBiotop::Les) {
                                    tempBunky[i][j - 1].setBiotop(PoziarBiotop::Poziar);
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
    this->bunky = tempBunky;
}

void Svet::regeneracia() {
    std::vector<std::vector<Bunka>> tempBunky = this->bunky;
    for (int i = 0; i < this->vyska; ++i) {
        for (int j = 0; j < this->sirka; ++j) {
            if (bunky[i][j].getBiotop() == PoziarBiotop::Poziar) {
                if (generator.dajPravdepodobnost() <= 0.10) {
                    if (j > 0) {
                        if (bunky[i][j - 1].getBiotop() == PoziarBiotop::Voda) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                            std::cout << "zmena" << i << j << "\n";
                        }
                    }
                    if (j < sirka - 1) {
                        if (bunky[i][j + 1].getBiotop() == PoziarBiotop::Voda) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                            std::cout << "zmena" << i << j << "\n";
                        }
                    }
                    if (i < vyska - 1) {
                        if (bunky[i + 1][j].getBiotop() == PoziarBiotop::Voda) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                            std::cout << "zmena" << i << j << "\n";
                        }
                    }
                    if (i > 0) {
                        if (bunky[i - 1][j].getBiotop() == PoziarBiotop::Voda) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Luka);
                            std::cout << "zmena" << i << j << "\n";
                        }
                    }
                }
            } else if (bunky[i][j].getBiotop() == PoziarBiotop::Luka) {
                if (generator.dajPravdepodobnost() <= 0.02) {
                    if (j > 0) {
                        if (bunky[i][j - 1].getBiotop() == PoziarBiotop::Les) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                            std::cout << "zmena" << i << j << "\n";
                        }
                    }
                    if (j < sirka - 1) {
                        if (bunky[i][j + 1].getBiotop() == PoziarBiotop::Les) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                            std::cout << "zmena" << i << j << "\n";
                        }
                    }
                    if (i < vyska - 1) {
                        if (bunky[i + 1][j].getBiotop() == PoziarBiotop::Les) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                            std::cout << "zmena" << i << j << "\n";
                        }
                    }
                    if (i > 0) {
                        if (bunky[i - 1][j].getBiotop() == PoziarBiotop::Les) {
                            tempBunky[i][j].setBiotop(PoziarBiotop::Les);
                            std::cout << "zmena " << i << " " << j << "\n";
                        }
                    }
                }
            }
        }
    }
    this->bunky = tempBunky;
}

void Svet::input() {
    while (true) {
        if (GetKeyState('P') & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            if (!pauza) {
                std::cout << "ZASTAVENE\n";
                pauza = true;
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




