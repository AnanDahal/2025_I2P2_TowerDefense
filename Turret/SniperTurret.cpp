#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/SniperBullet.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "SniperTurret.h"
#include "Scene/PlayScene.hpp"
#include "Enemy/Enemy.hpp"

const int SniperTurret::Price = 75;

SniperTurret::SniperTurret(float x, float y)
    : Turret("play/tower-base.png", "play/turret-7.png", x, y, 800, Price, 3, 100) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
    rotateRadian = 4 * ALLEGRO_PI;
}

void SniperTurret::CreateBullet() {
    if (!Target) return;

    Engine::Point enemyPos = Target->Position;
    Engine::Point enemyVelocity = Target->Velocity;

    float bulletSpeed = SniperBullet::getSpeed();
    Engine::Point toEnemy = enemyPos - Position;

    float distance = toEnemy.Magnitude();
    float timeToHit = distance / bulletSpeed;

    Engine::Point predictedPos = enemyPos + enemyVelocity * timeToHit;

    Engine::Point direction = (predictedPos - Position).Normalize();
    float rotation = atan2(direction.y, direction.x);

    getPlayScene()->BulletGroup->AddNewObject(
        new SniperBullet(Position + direction * 36, direction, rotation, this, buffed)
    );
    AudioHelper::PlayAudio("gun.wav");
}