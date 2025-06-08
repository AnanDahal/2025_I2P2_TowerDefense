#ifndef ENEMYBULLET_HPP
#define ENEMYBULLET_HPP

#include "Engine/Sprite.hpp"
namespace Engine { struct Point; }

class PlayScene;
class Turret;

class EnemyBullet : public Engine::Sprite {
private:
    float speed, damage;

    PlayScene *getPlayScene();

public:
    // now takes a sprite path
    EnemyBullet(const std::string &sprite,
                Engine::Point position,
                Engine::Point direction,
                float rotation,
                float speed,
                float damage);

    void Update(float deltaTime) override;
};

#endif // ENEMYBULLET_HPP
