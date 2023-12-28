#include <iostream>
#include "PoziarnyBiotop.h"

int main() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
             char poziar = static_cast<char>(PoziarBiotop::Voda);
             std::cout << poziar << " ";
        }
        std::cout << "\n";

    }
    return 0;
}
