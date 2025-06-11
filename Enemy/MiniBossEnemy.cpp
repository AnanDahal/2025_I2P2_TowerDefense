#include <string>
#include <iostream>
#include "BossEnemy.h"
#include "SoldierEnemy.hpp"
#include "ArmyEnemy.hpp"
#include "BiggerCarrierEnemy.h"
#include "CarrierEnemy.h"
#include "TankEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "MiniBossEnemy.h"



MiniBossEnemy::MiniBossEnemy(int x, int y, int ph) : Enemy("play/enemy-4.png", x, y, 10, 5, 10, 30) {
    phase = ph;
}

void MiniBossEnemy::OnExplode() {

    PlayScene* scene = getPlayScene();

    // Spawn all enemies immediately when exploding
    std::cout << "Spawning enemies immediately on explosion..." << std::endl;
    if (phase == 2) {
        // Spawn CarrierEnemies
        for (int i = 0; i < 10; i++) {
            CarrierEnemy* carrier = new CarrierEnemy(Position.x + (i * 20), Position.y);
            scene->EnemyGroup->AddNewObject(carrier);
            carrier->UpdatePath(scene->mapDistance);
        }

        // Spawn BiggerCarrierEnemies
        for (int i = 0; i < 10; i++) {
            BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + (i * 25), Position.y);
            scene->EnemyGroup->AddNewObject(bigger);
            bigger->UpdatePath(scene->mapDistance);
        }

        MiniBossEnemy* miniboss = new MiniBossEnemy(Position.x, Position.y, 1);
        scene->EnemyGroup->AddNewObject(miniboss);
        miniboss->UpdatePath(scene->mapDistance);
        std::cout<< "first death" << phase << std::endl;
    }
    else if (phase == 1) {
        // Spawn CarrierEnemies
        for (int i = 0; i < 10; i++) {
            CarrierEnemy* carrier = new CarrierEnemy(Position.x + (i * 20), Position.y);
            scene->EnemyGroup->AddNewObject(carrier);
            carrier->UpdatePath(scene->mapDistance);
        }

        // Spawn BiggerCarrierEnemies
        for (int i = 0; i < 10; i++) {
            BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + (i * 25), Position.y);
            scene->EnemyGroup->AddNewObject(bigger);
            bigger->UpdatePath(scene->mapDistance);
        }
        phase--;

    }

    std::cout << "Finished spawning all enemies" << std::endl;

    // Now call parent to handle normal explosion
    Enemy::OnExplode();

}