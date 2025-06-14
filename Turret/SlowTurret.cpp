#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/SlowBullet.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "SlowTurret.h" //

#include "Scene/PasswordScene.h"
#include "Scene/PlayScene.hpp"
#include "Scene/ShopScene.h"

const int SlowTurret::Price = 100;
SlowTurret::SlowTurret(float x, float y) : Turret("play/tower-base.png", GetTurretImage(6), x, y, 300, Price, 1, 500) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
    missThreshold = 0;
    isLocked = false;
}
void SlowTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new SlowBullet(Position + normalized * 36 - normal * 6, diff, rotation, this, buffed, missing, slowUpgrade1));
    getPlayScene()->BulletGroup->AddNewObject(new SlowBullet(Position + normalized * 36 + normal * 6, diff, rotation, this, buffed, missing, slowUpgrade1));
    missThreshold++;
    if (missThreshold == 2) {
        missing = false;
    }
    AudioHelper::PlayAudio("laser.wav"); //change sound
}

void SlowTurret::Update(float deltaTime) {
    Turret::Update(deltaTime);
    if (slowUpgrade2) {
        CollisionRadius *= 2;
    }
    if (slowUpgrade1) {
        coolDown /= 2;
    }
}
//
// Created by user on 5/16/2025.
//
