//
// Created by user on 6/12/2025.
//

#ifndef MISSENEMY_H
#define MISSENEMY_H


#include "Enemy.hpp"
#include "Engine/Sprite.hpp"
#include "Bullet/MissEnemyBullet.h"
#include "Engine/AudioHelper.hpp"

class MissEnemy : public Enemy {
private:
    Sprite head;
    float targetRotation;

    // shooting parameters
    float shootRadius   = 250.f;
    float shootCooldown =   2.0f;
    float shootReload   =   0.0f;
    int   shootDamage   =  0;
    float shootSpeed    = 400.0f;

    std::string bulletSprite;

public:
    // 1) default, uses a hard-coded sprite
    explicit MissEnemy(int x, int y, bool fast);
    // 2) full, lets you pass any sprite
    MissEnemy(int x, int y, const std::string &bulletSprite);

    void Draw()   const override;
    void Update(float deltaTime) override;
};

#endif //MISSENEMY_H
