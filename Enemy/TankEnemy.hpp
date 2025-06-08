#ifndef TANKENEMY_HPP
#define TANKENEMY_HPP

#include "Enemy.hpp"
#include "Engine/Sprite.hpp"
#include "Bullet/EnemyBullet.hpp"
#include "Engine/AudioHelper.hpp"

class TankEnemy : public Enemy {
private:
    Sprite head;
    float targetRotation;

    // shooting parameters
    float shootRadius   = 250.f;
    float shootCooldown =   2.0f;
    float shootReload   =   0.0f;
    int   shootDamage   =  100;
    float shootSpeed    = 400.0f;

    std::string bulletSprite;

public:
    // 1) default, uses a hard-coded sprite
    explicit TankEnemy(int x, int y);
    // 2) full, lets you pass any sprite
    TankEnemy(int x, int y, const std::string &bulletSprite);

    void Draw()   const override;
    void Update(float deltaTime) override;
};

#endif  // TANKENEMY_HPP
