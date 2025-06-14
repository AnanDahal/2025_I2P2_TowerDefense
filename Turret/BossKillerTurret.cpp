#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/BossKillerBullet.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "BossKillerTurret.h"
#include "Scene/PlayScene.hpp"
#include "Scene/ShopScene.h"

const int BossKillerTurret::Price = 500;
BossKillerTurret::BossKillerTurret(float x, float y)
    : Turret("play/tower-base.png", GetTurretImage(5), x, y, 200, Price, 0.5, 1000) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
    missThreshold = 0;
    isLocked = false;
}
void BossKillerTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new BossKillerBullet(Position + normalized * 36, diff, rotation, this, buffed, missing, bossUpgrade1));
    missThreshold++;
    if (missThreshold == 2) {
        missing = false;
    }
    AudioHelper::PlayAudio("kraber.wav");
}

void BossKillerTurret::Update(float deltaTime) {
    Turret::Update(deltaTime);
    if (bossUpgrade2) {
        CollisionRadius *= 2;
    }
}
