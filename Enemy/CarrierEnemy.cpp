#include <string>
#include <iostream>
#include "CarrierEnemy.h"
#include "SoldierEnemy.hpp"
#include "Scene/PlayScene.hpp"



CarrierEnemy::CarrierEnemy(int x, int y, bool fast) : Enemy("play/enemy-4.png", x, y, 10, 20, 50, 30) {
    if (fast) {
        speed *= 2;
    }
}

void CarrierEnemy::OnExplode() {
    Enemy::OnExplode();

    PlayScene* scene = getPlayScene();

    SoldierEnemy* soldier1 = new SoldierEnemy(Position.x + 10, Position.y, false);
    SoldierEnemy* soldier2 = new SoldierEnemy(Position.x - 10, Position.y, false);

    scene->EnemyGroup->AddNewObject(soldier1);
    scene->EnemyGroup->AddNewObject(soldier2);

    soldier1->UpdatePath(scene->mapDistance);
    soldier2->UpdatePath(scene->mapDistance);

    //std::cout << "spawning soldier\n";
}