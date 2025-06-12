//
// Created by user on 6/12/2025.
//

#ifndef BOSSKILLERBULLET_H
#define BOSSKILLERBULLET_H
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class BossKillerBullet : public Bullet {
public:
    explicit BossKillerBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff, bool miss);
    void OnExplode(Enemy *enemy) override;
};
#endif //BOSSKILLERBULLET_H
