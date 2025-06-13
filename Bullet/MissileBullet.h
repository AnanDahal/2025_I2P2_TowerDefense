//
// Created by Farrel on 6/12/2025.
//

#ifndef MISSILEBULLET_H
#define MISSILEBULLET_H

#include "AOEBullet.h"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class MissileBullet : public AOEBullet {
public:
    explicit MissileBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, float aoeRadius, bool buff, bool miss, bool upgrade);
    void OnExplode(Enemy *enemy) override;
};

#endif //MISSILEBULLET_H
