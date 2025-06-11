//
// Created by user on 5/25/2025.
//

#ifndef SNIPERBULLET_H
#define SNIPERBULLET_H
#include "Bullet.hpp"
class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class SniperBullet : public Bullet {
public:
    explicit SniperBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff, bool miss);
    void OnExplode(Enemy *enemy) override;
    static float getSpeed();
};
#endif //SNIPERBULLET_H
