//
// Created by user on 5/18/2025.
//
#include <allegro5/base.h>
#include <cmath>
#include <string>

 //no bullets
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "BuffTurret.h" //

#include "Scene/PasswordScene.h"
#include "Scene/PlayScene.hpp"
#include "Scene/ShopScene.h"

const int BuffTurret::Price = 400;
BuffTurret::BuffTurret(float x, float y) : Turret("play/tower-base.png", GetTurretImage(9), x, y, 300, Price, 1, 600) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
    isLocked = false;
}

void BuffTurret::CreateBullet() {
}

void BuffTurret::Update(float deltaTime) {
    // Call base Update for positioning, etc.
    Sprite::Update(deltaTime);
    imgBase.Position = Position;
    imgBase.Tint = Tint;

    if (!Enabled) return;

    PlayScene* scene = getPlayScene();
    for (auto& obj : scene->TowerGroup->GetObjects()) {
        Turret* turret = dynamic_cast<Turret*>(obj);
        if (turret == this) continue; // Don't buff self

        Engine::Point diff = turret->Position - Position;
        if (diff.Magnitude() <= CollisionRadius) {
            // Apply buff
            if (!turret->buffed ) {
                turret->buffed = true;
                if (buffUpgrade1) {
                    turret->coolDown /= 2;
                }
                if (buffUpgrade2) {
                    turret->missing = false;
                }
                turret->CollisionRadius *= 2;
            }
        }
    }
}