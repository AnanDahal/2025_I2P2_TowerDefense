//
// Created by user on 6/12/2025.
//

#ifndef TANKKILLERBULLET_H
#define TANKKILLERBULLET_H
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class TankKillerBullet : public Bullet {
public:
    explicit TankKillerBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff, bool miss);
    void OnExplode(Enemy *enemy) override;
};

#endif //TANKKILLERBULLET_H
