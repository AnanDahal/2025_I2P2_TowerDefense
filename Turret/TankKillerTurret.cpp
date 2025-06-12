#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/TankKillerBullet.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "TankKillerTurret.h" //

#include "Scene/PasswordScene.h"
#include "Scene/PlayScene.hpp"

const int TankKillerTurret::Price = 250;
TankKillerTurret::TankKillerTurret(float x, float y) : Turret("play/tower-base.png", "play/turret-6.png", x, y, 300, Price, 1, 200) {
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
void TankKillerTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new TankKillerBullet(Position + normalized * 36 - normal * 6, diff, rotation, this, buffed, missing));
    getPlayScene()->BulletGroup->AddNewObject(new TankKillerBullet(Position + normalized * 36 + normal * 6, diff, rotation, this, buffed, missing));
    missThreshold++;
    if (missThreshold == 2) {
        missing = false;
    }
    AudioHelper::PlayAudio("laser.wav"); //change sound
}//
// Created by user on 6/12/2025.
//
