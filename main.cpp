#include <iostream>
#include "PoziarnyBiotop.h"
#include "Bunka.h"

int main() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
             Bunka bunka = Bunka(0, 0, PoziarBiotop::Voda);
             std::cout << bunka.getZnak() << " ";
        }
        std::cout << "\n";

    }
    return 0;
}
