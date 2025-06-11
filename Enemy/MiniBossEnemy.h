//
// Created by user on 6/11/2025.
//
#include "Enemy.hpp"
#ifndef MINIBOSSENEMY_H
#define MINIBOSSENEMY_H


class MiniBossEnemy : public Enemy {
public:
    MiniBossEnemy(int x, int y);
    void Update(float deltaTime) override;
protected:
    void OnExplode() override;
    bool exploded = false;
    float spawnTimer = 0.0f;
    int spawnIndex = 0;
    int enemy1left = 10;
    int enemy2left = 10;
    int phase = 2;
};


#endif //MINIBOSSENEMY_H
