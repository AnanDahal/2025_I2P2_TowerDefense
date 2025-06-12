#include <string>

#include "SoldierEnemy.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
SoldierEnemy::SoldierEnemy(int x, int y) : Enemy("play/enemy-1.png", 0, x, y, 10, 50, 5, 5) {
}

void SoldierEnemy::Hit(float damage, bool slow) {
    Enemy::Hit(damage, slow);
    SetInvisible(2.0f);
}


void SoldierEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
}


void SoldierEnemy::Draw() const {
    Enemy::Draw();
}

