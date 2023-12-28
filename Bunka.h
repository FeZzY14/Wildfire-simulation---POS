//
// Created by jmisa on 28. 12. 2023.
//

#ifndef SEMPRACA2_BUNKA_H
#define SEMPRACA2_BUNKA_H
#include "PoziarnyBiotop.h"

class Bunka {
private:
    int x;
    int y;
    PoziarBiotop biotop;
    char znak;
public:
    Bunka(int x, int y, PoziarBiotop biotop);
    int getX();
    int getY();
    void setX(int newX);
    void setY(int newY);
    char getZnak();
    PoziarBiotop getBiotop();
    void setBiotop(PoziarBiotop newBiotop);
};


#endif //SEMPRACA2_BUNKA_H
