//
// Created by spotk on 28. 12. 2023.
//
#include "Bunka.h"
#include "Svet.h"
#include <vector>
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <windows.h>


Svet::Svet(int sirka, int vyska) {
    this->sirka = sirka;
    this->vyska = vyska;
    this->pauza = false;
    this->vietor = generator.dajVietor();
    this->bunky.resize(this->vyska, std::vector<Bunka>(this->sirka));
    this->pocetSimulacii = 0;
}

void Svet::vytvorSvet() {
    for (int i = 0; i < this->vyska; i++) {
        for (int j = 0; j < this->sirka; j++) {
            PoziarBiotop biotop = this->generator.dajNahodnyBiotop();
            this->bunky[i][j] = Bunka(j, i, biotop);
        }
    }
}

void Svet::vytvorSvetZoSuboru(const std::string& nazovSuboru) {
    std::ifstream subor;
    subor.open(nazovSuboru);

    std::string riadok;
    std::vector<std::string> svet;
    int vyskaSvet = 0;
    if (subor.is_open()) {
        while (subor) {
            std::getline(subor,riadok);
            vyskaSvet++;
            svet.push_back(riadok);
        }
    }
    this->vyska = vyskaSvet - 2;
    svet.pop_back();
    this->sirka = svet[0].length();
    this->bunky.resize(this->vyska, std::vector<Bunka>(this->sirka));
    this->pocetSimulacii = 0;

    for (int i = 0; i < this->vyska; i++) {
        for (int j = 0; j < this->sirka; j++) {
            PoziarBiotop biotop = this->dajBiotomZoZnaku(svet[i][j]);
            bunky[i][j] = Bunka(j, i, biotop);
        }
    }
    switch (svet[svet.size() - 1][0]) {
        case '|': this->vietor = Vietor::Bezvetrie;break;
        case '^': this->vietor = Vietor::Hore;break;
        case 'v': this->vietor = Vietor::Dole;break;
        case '<': this->vietor = Vietor::Vlavo;break;
        case '>': this->vietor = Vietor::Vpravo;break;
    }
}

void Svet::vytvorPoziarRandomPosition() {
    int x = this->generator.dajNahodnyPoziar(this->vyska);
    int y = this->generator.dajNahodnyPoziar(this->sirka);
    this->bunky[x][y].setBiotop(PoziarBiotop::Poziar);
}

void Svet::vypisSvet() {
    std::cout << "Smer vetra: " << static_cast<char>(this->vietor) << "\n";
    for (int i = 0; i < this->vyska; ++i) {
        for (int j = 0; j < this->sirka; ++j) {
            std::cout << this->bunky[i][j].getZnak() << " ";
        }
        std::cout << std::endl;
    }
}


void Svet::spustiPoziar() {
    while (true) {
        if (pocetSimulacii % 3 == 0 && pocetSimulacii > 0 && this->vietor != Vietor::Bezvetrie) {
            this->vietor = generator.dajSmerVetra();
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::unique_lock<std::mutex> lock(this->mutex);
        this->vypisSvet();
        while (pauza) {
            this->stop.wait(lock);
        }
        this->sireniePoziaru();
        lock.unlock();
        pocetSimulacii++;
    }
}

void Svet::spustiRegeneraciu() {
    while (true) {
        if (pocetSimulacii > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::unique_lock<std::mutex> lock(this->mutex);
            while (pauza) {
                this->stop.wait(lock);
            }
            this->regeneraciaBiotopu();
            lock.unlock();
        }
    }
}
void Svet::regeneraciaBiotopu() {
    std::vector<std::vector<Bunka>> tempBunky = this->bunky;
    std::vector<std::string> tempZmenaSpravy;
    for (int i = 0; i < this->vyska; ++i) {
        for (int j = 0; j < this->sirka; ++j) {
            if (bunky[i][j].getBiotop() == PoziarBiotop::Poziar) {
                double prav = generator.dajPravdepodobnost();
                if (prav <= 0.10) {
                    if (this->vOkoli(bunky[i][j], PoziarBiotop::Voda)) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Luka);

                        std::cout << "Zhoreny biotop '#' sa zmenil na biotop '.' (Luka) na suradniciach: [" << std::to_string(i) << "; "
                                                  << std::to_string(j) << "] s pravdepodbnostou: " << std::to_string(prav) + ".\n";

//                        tempZmenaSpravy.push_back("Zhoreny biotop '#' sa zmenil na biotop '.' (Luka) na suradniciach: [" + std::to_string(i) + "; "
//                                                  + std::to_string(j) + "] s pravdepodbnostou: " + std::to_string(prav) + ".");
                    }
                }

            } else if (bunky[i][j].getBiotop() == PoziarBiotop::Luka) {
                double pravdepodobnost = generator.dajPravdepodobnost();
                if (pravdepodobnost <= 0.02) {
                    if (this->vOkoli(bunky[i][j], PoziarBiotop::Les)) {
                        tempBunky[i][j].setBiotop(PoziarBiotop::Les);

                        std::cout << "Biotop '.' (Luka) sa zmenil na biotop 'T' (Les) na suradniciach: [" << std::to_string(i) << "; "
                                                  << std::to_string(j) + "] s pravdepodbnostou: " << std::to_string(pravdepodobnost ) << ".\n";

//                        tempZmenaSpravy.push_back("Biotop '.' (Luka) sa zmenil na biotop 'T' (Les) na suradniciach: [" + std::to_string(i) + "; "
//                                                  + std::to_string(j) + "] s pravdepodbnostou: " + std::to_string(pravdepodobnost ) + ".");
                    }
                }
            }
        }
    }
    this->bunky = tempBunky;
//    for (const auto& message : tempZmenaSpravy) {
//        std::cout << message << "\n";
//    }
}

bool Svet::vOkoli(Bunka bunka, PoziarBiotop biotop) {
    int i = bunka.getY();
    int j = bunka.getX();
    if (j > 0 && bunky[i][j - 1].getBiotop() == biotop) {
        return true;
    } else if (j < sirka - 1 && bunky[i][j + 1].getBiotop() == biotop) {
        return true;
    } else if (i < vyska - 1 && bunky[i + 1][j].getBiotop() == biotop) {
        return true;
    } else if (i > 0 && bunky[i - 1][j].getBiotop() == biotop) {
        return true;
    }
    return false;
}


void Svet::inputPause() {
    while (true) {
        if (GetKeyState('P') & 0x8000) {
            if (!pauza) {
                pauza = true;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout << "---POZASTAVENA SIMULACIA--- \n";
                do {
                    std::cout << "poziar : Zaloz poziar na danych suradniciach\n";
                    std::cout << "ulozLok : Lokalne uloz mapu sveta do suboru\n";
                    std::cout << "nacitajLok : nacitaj mapu zo suboru a spusti novu simulaciu\n";
                    std::cout << "ulozServ : Uloz mapu sveta na server\n";
                    std::cout << "pokracuj : Pokracuj v simulacii\n";
                    std::cout << "ukonci : Ukonci simulaciu(program)\n";
                    std::string volba;
                    do {
                        std::cout << ">";
                        std::cin >> volba;
                        if (volba == "poziar") {
                            std::cout << "Zvolili ste si zalozenie poziaru!\n";
                            std::cout << "Zadajte prvu suradnicu (vysku): \n";
                            std::string vyskaSuradnica;
                            int surVyskaInt;
                            do {
                                std::cout << "> ";
                                std::cin >> vyskaSuradnica;
                                std::cout << std::endl;
                                try {
                                    surVyskaInt = std::stoi(vyskaSuradnica);
                                    if (surVyskaInt < 0 || surVyskaInt > this->vyska - 1) {
                                        throw std::out_of_range("Mimo sveta");
                                    }
                                    std::cout << "Zadali ste: " << vyskaSuradnica << "\n";
                                    break;
                                } catch (...) {
                                    std::cout << "Zla suradnica, zadali ste suradnicu mimo svet!! Zadajte znova! \n";
                                }
                            } while (true);
                            std::cout << "Zadajte druhu suradnicu (sirku): \n";
                            std::string sirkaSuradnica;
                            int surSirkaInt;
                            do {
                                std::cout << "> ";
                                std::cin >> sirkaSuradnica;
                                std::cout << std::endl;
                                try {
                                    surSirkaInt = std::stoi(sirkaSuradnica);
                                    if (surSirkaInt < 0 || surSirkaInt > this->sirka - 1) {
                                        throw std::out_of_range("Mimo sveta");
                                    }
                                    std::cout << "Zadali ste: " << sirkaSuradnica << "\n";
                                    break;
                                } catch (...) {
                                    std::cout << "Zla suradnica, zadali ste suradnicu mimo svet!! Zadajte znova! \n";
                                }
                            } while (true);
                            std::cout << "Zalozili ste poziar na suradniciach [" << surVyskaInt << "; " << surSirkaInt << "]."<< "\n";
                            this->bunky[surVyskaInt][surSirkaInt].setBiotop(PoziarBiotop::Poziar);
                            break;
                        } else if (volba == "pokracuj") {
                            if (pauza) {
                                std::cout << "---SPUSTENA SIMULACIA---\n";
                                pauza = false;
                                stop.notify_all();
                            }
                            break;

                        } else if (volba == "ukonci") {
                            std::cout << "Ukoncili ste simulaciu(program)!!!\n---KONIEC PROGRAMU---" << std::endl;
                            exit(0);
                        } else if (volba == "ulozLok"){
                            do {
                                std::cout << "Zvolili ste si ulozenie svetu do suboru!\n";
                                std::cout << "Zadajte nazov suboru (bez pripony): \n";
                                std::string nazovSub;
                                std::cout << ">";
                                std::cin >> nazovSub;
                                nazovSub += ".txt";
                                struct stat buf{};
                                if (stat(nazovSub.c_str(), &buf) != -1) {
                                    std::string moznost;
                                    do {
                                        std::cout << "Subor uz existuje chcete ho prepisat? \n";
                                        std::cout << "ano : subor bude prepisany \n";
                                        std::cout << "nie : nevykona sa ziadna akcia\n";
                                        std::cout << ">";
                                        std::cin >> moznost;
                                        if (moznost == "ano") {
                                            this->ulozSvetDoSuboru(nazovSub);
                                            if (this->ulozSvetDoSuboru(nazovSub) == -1) {
                                                std::cerr << "chyba pri vytvarani suboru!!\n";
                                            } else {
                                                std::cout << "subor bol uspesne prepisany!!\n";
                                                std::cout << std::endl;
                                                break;
                                            }
                                            std::cout << std::endl;
                                            break;
                                        } else if (moznost == "nie"){
                                            break;
                                        } else {
                                            std::cout << "zla volba!!\n";
                                        }
                                    } while (moznost != "nie");
                                    if (moznost == "ano"){
                                        break;
                                    }
                                } else {
                                    if (this->ulozSvetDoSuboru(nazovSub) == -1) {
                                        std::cerr << "chyba pri vytvarani suboru!!\n";
                                    } else {
                                        std::cout << "subor bol uspesne vytvoreny!!\n";
                                        std::cout << std::endl;
                                        break;
                                    }
                                }
                            } while (true);
                            break;
                        } else if (volba == "nacitajLok") {
                            do {
                                std::cout << "Zvolili ste si nacitanie svetu zo suboru!\n";
                                std::cout << "Zadajte nazov suboru (bez pripony): \n";
                                std::string nazovSub;
                                std::cout << ">";
                                std::cin >> nazovSub;
                                nazovSub += ".txt";
                                struct stat buf{};
                                if (stat(nazovSub.c_str(), &buf) == -1) {
                                    std::cout << "Subor neexistuje, zadajte iny subor!! \n";
                                } else {
                                    std::cout << "subor bol uspesne nacitany\n";
                                    this->vytvorSvetZoSuboru(nazovSub);
                                    std::cout << std::endl;
                                    break;
                                }
                            } while (true);
                            break;
                        } else {
                            std::cout << "Zle zadana volba!! Zadajte znova!\n";
                        }
                    } while (volba != "poziar" || volba != "ulozLok" || volba != "ulozServ" || volba != "pokracuj" || volba != "ukonci");
                } while (pauza);
            }
        }

        if (GetKeyState('A') & 0x8000) {
            if (pauza) {
                std::cout << "---SPUSTENA SIMULACIA---\n";
                pauza = false;
                stop.notify_all();
            }
        }
    }
}

void Svet::sireniePoziaru() {
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
}

int Svet::ulozSvetDoSuboru(const std::string& fileName) {

    std::ofstream subor(fileName, std::ofstream::out | std::ofstream::trunc);
    if (!subor) {
        return -1;
    }

    for (int i = 0; i < this->vyska; i++) {
        for (int j = 0; j < this->sirka; j++) {
            subor << bunky[i][j].getZnak();
        }
        subor << "\n";
    }
    subor << this->dajZnakVetra(this->vietor);
    subor.close();
    return 1;
}

PoziarBiotop Svet::dajBiotomZoZnaku(char znak) {
    switch (znak) {
        case 'T': return PoziarBiotop::Les;break;
        case '.': return PoziarBiotop::Luka;break;
        case 'O': return PoziarBiotop::Skala;break;
        case '~': return PoziarBiotop::Voda;break;
        case '#': return PoziarBiotop::Poziar;break;
    }
    return PoziarBiotop::Poziar;
}

char Svet::dajZnakVetra(Vietor vietor) {
    switch (vietor) {
        case Vietor::Bezvetrie: return '|';break;
        case Vietor::Hore: return '^';break;
        case Vietor::Dole: return 'v';break;
        case Vietor::Vlavo: return '<';break;
        case Vietor::Vpravo: return '>';break;
        default:return '-';break;
    }
}

