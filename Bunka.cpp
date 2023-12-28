//
// Created by jmisa on 28. 12. 2023.
//

#include "Bunka.h"

int Bunka::getX() {
    return this->x;
}

int Bunka::getY() {
    return this->y;
}

void Bunka::setX(int newX) {
    this->x = newX;
}

void Bunka::setY(int newY) {
    this->y = newY;
}

char Bunka::getZnak() {
    return static_cast<char>(this->biotop);
}

PoziarBiotop Bunka::getBiotop() {
    return this->biotop;
}

void Bunka::setBiotop(PoziarBiotop newBiotop) {
    this->biotop = newBiotop;
    this->znak = static_cast<char>(this->biotop);
}

Bunka::Bunka(int x, int y, PoziarBiotop biotop) :x(x), y(y), biotop(biotop) {}

Bunka::Bunka() {

}
