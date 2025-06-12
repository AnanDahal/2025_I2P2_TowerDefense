//
// Created by Farrel on 6/12/2025.
//

#ifndef AOEBULLET_H
#define AOEBULLET_H
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class AOEBullet : public Engine::Sprite {
protected:
    float speed;
    float damage;
    float aoeRadius;
    bool isBuffed;
    bool isSlow;
    bool isMiss;
    Turret *parent;
    PlayScene *getPlayScene();
    virtual void OnExplode(Enemy *enemy);

public:
    Enemy *Target = nullptr;
    explicit AOEBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, float aoeRadius, bool buff, bool slow, bool miss);
    void Update(float deltaTime) override;
};

#endif //AOEBULLET_H
