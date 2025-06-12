//
// Created by Farrel on 6/12/2025.
//

#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/MissileBullet.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "MissileTurret.h"

const int MissileTurret::Price = 150;
MissileTurret::MissileTurret(float x, float y)
    : Turret("play/tower-base.png", "play/turret-4.png", x, y, 300, Price, 1.5, 500) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
    missThreshold = 0;
    if (OnStage >= 4) {
        isLocked = false;
    }
    else {
        isLocked = true;
    }
}
void MissileTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position + normalized * 36, diff, rotation, this, 50, buffed, missing));
    missThreshold++;
    if (missThreshold == 2) {
        missing = false;
    }
    AudioHelper::PlayAudio("gun.wav");
}
