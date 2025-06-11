#include <string>

#include "ArmyEnemy.hpp"

ArmyEnemy::ArmyEnemy(int x, int y, bool fast) : Enemy("play/enemy-2.png", x, y, 15, 30, 10, 10) {
    if (fast) {
        speed *= 2;
    }
}
