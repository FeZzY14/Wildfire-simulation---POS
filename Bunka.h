//
// Created by jmisa on 28. 12. 2023.
//

#ifndef WildfiresimulationPOS_BUNKA_H
#define WildfiresimulationPOS_BUNKA_H
#include "PoziarnyBiotop.h"

class Bunka {
private:
    int x;
    int y;
    PoziarBiotop biotop;
    char znak;
public:
    Bunka();
    Bunka(int x, int y, PoziarBiotop biotop);
    int getX();
    int getY();
    void setX(int newX);
    void setY(int newY);
    char getZnak();
    PoziarBiotop getBiotop();
    void setBiotop(PoziarBiotop newBiotop);
};

#endif //WildfiresimulationPOS_BUNKA_H
