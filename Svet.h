//
// Created by spotk on 28. 12. 2023.
//

#include "Bunka.h"
#include <vector>
#include <cstdlib>
#include <thread>
#include <mutex>
#include "Generator.h"
#include "Vietor.h"
#include "Client.h"
#include <mutex>
#include <condition_variable>

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
    Client client;
    bool connected = false;
public:
    Svet(int sirka, int vyska);
    void vytvorSvet();
    void vytvorSvetZoSuboru(const std::string& nazovSuboru);
    void vytvorPoziarRandomPosition();
    void vypisSvet();
    void spustiPoziar();
    void spustiRegeneraciu();
    void sireniePoziaru();
    void regeneraciaBiotopu();
    void inputPause();
    bool vOkoli(Bunka bunka, PoziarBiotop biotop);
    int ulozSvetDoSuboru(const std::string& fileName);
    PoziarBiotop dajBiotomZoZnaku(char znak);
    char dajZnakVetra(Vietor vietor);
};

