//
// Created by user on 6/11/2025.
//

#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "Enemy.hpp"

class BossEnemy : public Enemy {
public:
    BossEnemy(int x, int y);
    void Update(float deltaTime) override;
protected:
    void OnExplode() override;
    bool exploded = false;
    float spawnTimer = 0.0f;
    int spawnIndex = 0;
    int enemy1left = 50;
    int enemy2left = 50;
    int phase = 5;
};

#endif //BOSSENEMY_H
