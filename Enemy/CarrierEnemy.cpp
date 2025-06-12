#include <string>
#include <iostream>
#include "CarrierEnemy.h"
#include "SoldierEnemy.hpp"
#include "Scene/PlayScene.hpp"



CarrierEnemy::CarrierEnemy(int x, int y) : Enemy("play/enemy-4.png", 1, x, y, 10, 20, 50, 30) {
}

void CarrierEnemy::OnExplode() {
    Enemy::OnExplode();

    PlayScene* scene = getPlayScene();

    SoldierEnemy* soldier1 = new SoldierEnemy(Position.x + 10, Position.y);
    SoldierEnemy* soldier2 = new SoldierEnemy(Position.x - 10, Position.y);

    scene->EnemyGroup->AddNewObject(soldier1);
    scene->EnemyGroup->AddNewObject(soldier2);

    soldier1->UpdatePath(scene->mapDistance);
    soldier2->UpdatePath(scene->mapDistance);

    //std::cout << "spawning soldier\n";
}