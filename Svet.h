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
#include "Server.h"
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
    bool exit = false;
    Server server;
public:
    Svet(int sirka, int vyska, Server &server);
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
    PoziarBiotop dajBiotopZoZnaku(char znak);
    char dajZnakVetra(Vietor vietor);
    void SvetUlozNaServer(const std::string &nazovSvetu);
    int SvetJeNaServeri(const std::string &nazovSvetu);
    void vytvorSvetZoStrinngu(const std::string &svetString);
    void nacitanieSvetuZoServera();
};

