//
// Created by user on 6/10/2025.
//

#ifndef SLOWBULLET_H
#define SLOWBULLET_H
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class SlowBullet : public Bullet {
public:
    explicit SlowBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff, bool miss, bool upgrade);
    void OnExplode(Enemy *enemy) override;
};

#endif //SLOWBULLET_H
