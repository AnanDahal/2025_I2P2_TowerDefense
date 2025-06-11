//
// Created by user on 6/12/2025.
//

#ifndef MISSENEMYBULLET_H
#define MISSENEMYBULLET_H
#include "Engine/Sprite.hpp"
#include "EnemyBullet.hpp"
namespace Engine { struct Point; }

class PlayScene;
class Turret;

class MissEnemyBullet : public EnemyBullet {
private:
    float speed, damage;

    PlayScene *getPlayScene();

public:
    // now takes a sprite path
    MissEnemyBullet(Engine::Point position,Engine::Point direction,float rotation, float speed, float damage);
};


#endif //MISSENEMYBULLET_H
