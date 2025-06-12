//
// Created by user on 5/22/2025.
//
#include <string>
#include <iostream>
#include "CarrierEnemy.h"
#include "Scene/PlayScene.hpp"
#include "BiggerCarrierEnemy.h"



BiggerCarrierEnemy::BiggerCarrierEnemy(int x, int y) : Enemy("play/enemy-6.png", 4, x, y, 10, 15, 100, 80) {
}

void BiggerCarrierEnemy::OnExplode() {
    Enemy::OnExplode();

    PlayScene* scene = getPlayScene();

    CarrierEnemy* soldier1 = new CarrierEnemy(Position.x + 10, Position.y);
    CarrierEnemy* soldier2 = new CarrierEnemy(Position.x - 10, Position.y);

    scene->EnemyGroup->AddNewObject(soldier1);
    scene->EnemyGroup->AddNewObject(soldier2);

    soldier1->UpdatePath(scene->mapDistance);
    soldier2->UpdatePath(scene->mapDistance);

    //std::cout << "spawning soldier\n";
}
