#include <string>
#include <iostream>
#include "BossEnemy.h"
#include "SoldierEnemy.hpp"
#include "ArmyEnemy.hpp"
#include "BiggerCarrierEnemy.h"
#include "CarrierEnemy.h"
#include "TankEnemy.hpp"
#include "Scene/PlayScene.hpp"



BossEnemy::BossEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 10, 5, 1000, 30) {
}

void BossEnemy::OnExplode() {
    Enemy::OnExplode();
    exploded = true;
    enemy1left = 50;//reset
    enemy2left = 50;

}

void BossEnemy::Update(float deltaTime) {
    PlayScene* scene = getPlayScene();
    Enemy::Update(deltaTime);
    if (exploded) {
        spawnTimer += deltaTime;
        if (spawnTimer >= 0.5f) {
            spawnTimer = 0.0f;
            if (enemy1left > 0) {
                if (phase == 5) {
                    SoldierEnemy* soldier = new SoldierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(soldier);
                    soldier->UpdatePath(scene->mapDistance);
                    enemy1left--;
                }
                else if (phase == 4) {
                    ArmyEnemy* army = new ArmyEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(army);
                    army->UpdatePath(scene->mapDistance);
                    enemy1left--;
                }
                else if (phase == 3) {
                    // TankEnemy* tank = new TankEnemy(Position.x + 10, Position.y);
                    // scene->EnemyGroup->AddNewObject(tank);
                    // tank->UpdatePath(scene->mapDistance);
                    // enemy1left--;
                    //confuse enemy here
                    enemy1left--;
                }
                else if (phase == 2) {
                    CarrierEnemy* carrier = new CarrierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(carrier);
                    carrier->UpdatePath(scene->mapDistance);
                    enemy1left--;
                }
                else if (phase == 1) {
                    //miniboss
                    enemy1left--;
                }
            }
            else if (enemy2left > 0) {
                if (phase == 5) {
                    ArmyEnemy* army = new ArmyEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(army);
                    army->UpdatePath(scene->mapDistance);
                    enemy2left--;
                }
                else if (phase == 4) {
                    TankEnemy* tank = new TankEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(tank);
                    tank->UpdatePath(scene->mapDistance);
                    enemy2left--;
                }
                else if (phase == 3) {
                    CarrierEnemy* carrier = new CarrierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(carrier);
                    carrier->UpdatePath(scene->mapDistance);
                    enemy2left--;
                }
                else if (phase == 2) {
                    BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(bigger);
                    bigger->UpdatePath(scene->mapDistance);
                    enemy2left--;
                }
                else if (phase == 1) {
                    //miniboss
                    enemy2left--;
                }
            }
            else {
                if (phase > 1) {
                    BossEnemy* boss = new BossEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(boss);
                    boss->UpdatePath(scene->mapDistance);
                    phase--;
                }
                exploded = false;
            }
        }
    }
}