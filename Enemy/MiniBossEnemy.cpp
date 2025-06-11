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



MiniBossEnemy::MiniBossEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 10, 5, 500, 30) {
}

void MiniBossEnemy::OnExplode() {
    Enemy::OnExplode();
    exploded = true;
    enemy1left = 10;//reset
    enemy2left = 10;
}

void MiniBossEnemy::Update(float deltaTime) {
    PlayScene* scene = getPlayScene();
    Enemy::Update(deltaTime);
    if (exploded) {
        spawnTimer += deltaTime;
        if (spawnTimer >= 0.5f) {
            spawnTimer = 0.0f;
            if (enemy1left > 0) {
                if (phase == 2) {
                    CarrierEnemy* carrier = new CarrierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(carrier);
                    carrier->UpdatePath(scene->mapDistance);
                    enemy1left--;
                }
                else if (phase == 1) {
                    CarrierEnemy* carrier = new CarrierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(carrier);
                    carrier->UpdatePath(scene->mapDistance);
                    enemy1left--;
                }
            }
            else if (enemy2left > 0) {
                if (phase == 2) {
                    BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(bigger);
                    bigger->UpdatePath(scene->mapDistance);
                    enemy2left--;
                }
                else if (phase == 1) {
                    BiggerCarrierEnemy* bigger = new BiggerCarrierEnemy(Position.x + 10, Position.y);
                    scene->EnemyGroup->AddNewObject(bigger);
                    bigger->UpdatePath(scene->mapDistance);
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