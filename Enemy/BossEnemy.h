//
// Created by user on 6/11/2025.
//

#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "Enemy.hpp"

class BossEnemy : public Enemy {
public:
    BossEnemy(int x, int y, int ph);
protected:
    void OnExplode() override;
    int phase;
};

#endif //BOSSENEMY_H
