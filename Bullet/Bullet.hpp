#ifndef BULLET_HPP
#define BULLET_HPP
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class Bullet : public Engine::Sprite {
protected:
    float speed;
    float damage;
    bool isBuffed;
    bool isSlow;
    bool isMiss;
    Turret *parent;
    PlayScene *getPlayScene();
    virtual void OnExplode(Enemy *enemy);

public:
    Enemy *Target = nullptr;
    explicit Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff, bool slow, bool miss);
    void Update(float deltaTime) override;
};
#endif   // BULLET_HPP
