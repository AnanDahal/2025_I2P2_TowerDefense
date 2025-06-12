//
// Created by user on 6/11/2025.
//
#include "Enemy.hpp"
#ifndef MINIBOSSENEMY_H
#define MINIBOSSENEMY_H


class MiniBossEnemy : public Enemy {
public:
    MiniBossEnemy(int x, int y, int ph);
protected:
    void OnExplode() override;
    int phase;
};

#endif //MINIBOSSENEMY_H
