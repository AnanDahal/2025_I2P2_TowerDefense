#include <string>
#include <iostream>
#include "BossEnemy.h"
#include "SoldierEnemy.hpp"
#include "ArmyEnemy.hpp"
#include "BiggerCarrierEnemy.h"
#include "CarrierEnemy.h"
#include "MiniBossEnemy.h"
#include "MissEnemy.h"
#include "TankEnemy.hpp"
#include "Scene/PlayScene.hpp"



BossEnemy::BossEnemy(int x, int y, int ph) : Enemy("play/enemy-8.png", 4, x, y, 10, 5, 1000, 30) {
    phase = ph;
}

void BossEnemy::OnExplode() {

    PlayScene* scene = getPlayScene();

    // Spawn all enemies immediately when exploding
    if (phase == 6) {
        // Spawn CarrierEnemies
        for (int i = 0; i < 50; i++) {
            SoldierEnemy* soldier = new SoldierEnemy(Position.x + (i * 20), Position.y);
            scene->EnemyGroup->AddNewObject(soldier);
            soldier->UpdatePath(scene->mapDistance);
        }

        // Spawn BiggerCarrierEnemies
        for (int i = 0; i < 50; i++) {
            ArmyEnemy* army = new ArmyEnemy(Position.x + (i * 25), Position.y);
            scene->EnemyGroup->AddNewObject(army);
            army->UpdatePath(scene->mapDistance);
        }

        BossEnemy* boss = new BossEnemy(Position.x, Position.y, 5);
        scene->EnemyGroup->AddNewObject(boss);
        boss->UpdatePath(scene->mapDistance);
    }
    else if (phase == 5) {
        for (int i = 0; i < 30; i++) {
            TankEnemy* tank = new TankEnemy(Position.x + (i * 20), Position.y);
            scene->EnemyGroup->AddNewObject(tank);
            tank->UpdatePath(scene->mapDistance);
        }

        // Spawn CarrierEnemies
        for (int i = 0; i < 30; i++) {
            CarrierEnemy* carrier = new CarrierEnemy(Position.x + (i * 25), Position.y);
            scene->EnemyGroup->AddNewObject(carrier);
            carrier->UpdatePath(scene->mapDistance);
        }

        BossEnemy* boss = new BossEnemy(Position.x, Position.y, 4);
        scene->EnemyGroup->AddNewObject(boss);
        boss->UpdatePath(scene->mapDistance);
    }
    else if (phase == 4) {
        for (int i = 0; i < 30; i++) {
             MissEnemy* misser = new MissEnemy(Position.x + (i * 20), Position.y);
             scene->EnemyGroup->AddNewObject(misser);
             misser->UpdatePath(scene->mapDistance);
        }
        // Spawn BiggerCarrierEnemies
        for (int i = 0; i < 30; i++) {
            BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + (i * 25), Position.y);
            scene->EnemyGroup->AddNewObject(bigger);
            bigger->UpdatePath(scene->mapDistance);
        }

        BossEnemy* boss = new BossEnemy(Position.x, Position.y, 3);
        scene->EnemyGroup->AddNewObject(boss);
        boss->UpdatePath(scene->mapDistance);
    }
    else if (phase == 3) {
        for (int i = 0; i < 25; i++) {
             CarrierEnemy* carrier = new CarrierEnemy(Position.x + (i * 20), Position.y);
             scene->EnemyGroup->AddNewObject(carrier);
             carrier->UpdatePath(scene->mapDistance);
            //fast carrier
        }
        // Spawn BiggerCarrierEnemies
        for (int i = 0; i < 25; i++) {
            BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + (i * 25), Position.y);
            scene->EnemyGroup->AddNewObject(bigger);
            bigger->UpdatePath(scene->mapDistance);
            //fast bigger carrier
        }

        BossEnemy* boss = new BossEnemy(Position.x, Position.y, 2);
        scene->EnemyGroup->AddNewObject(boss);
        boss->UpdatePath(scene->mapDistance);
    }
    else if (phase == 2) {
        for (int i = 0; i < 30; i++) {
            BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + (i * 20), Position.y);
            scene->EnemyGroup->AddNewObject(bigger);
            bigger->UpdatePath(scene->mapDistance);
            //fast carrier
        }
        for (int i = 0; i < 2; i++) {
            MiniBossEnemy* mini = new MiniBossEnemy(Position.x + (i * 25), Position.y, 2);
            scene->EnemyGroup->AddNewObject(mini);
            mini->UpdatePath(scene->mapDistance);
        }

        BossEnemy* boss = new BossEnemy(Position.x, Position.y, 1);
        scene->EnemyGroup->AddNewObject(boss);
        boss->UpdatePath(scene->mapDistance);
    }
    else if (phase == 1) {
        for (int i = 0; i < 5; i++) {
            MiniBossEnemy* miniboss = new MiniBossEnemy(Position.x + (i * 20), Position.y, 2);
            scene->EnemyGroup->AddNewObject(miniboss);
            miniboss->UpdatePath(scene->mapDistance);
        }
    }

    std::cout << "Finished spawning all enemies" << std::endl;

    // Now call parent to handle normal explosion
    Enemy::OnExplode();
}

