
#include "include.h"
#include <chrono>

int main() {
    Card card = TEN, card2 = TWO;
    if (card >= card2) {
        std::cout << "equal" << std::endl;
    }
    else {
        std::cout << "not equal" << std::endl;
    }
    return 0;
}