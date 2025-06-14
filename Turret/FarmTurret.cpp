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
#include "FarmTurret.h" //

#include "Scene/PasswordScene.h"
#include "Scene/PlayScene.hpp"
#include "Scene/ShopScene.h"

const int FarmTurret::Price = 400;
FarmTurret::FarmTurret(float x, float y) : Turret("play/tower-base.png", GetTurretImage(10), x, y, 300, Price, 1, 600) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
    isLocked = false;
}

void FarmTurret::CreateBullet() {
}

void FarmTurret::Update(float deltaTime) {
    if (farmUpgrade1) {
        moneyPerHarvest = 20;
    }
    if (farmUpgrade2) {
        harvestInterval = 2.0f;
    }
    // Update base sprite logic
    Sprite::Update(deltaTime);
    imgBase.Position = Position;
    imgBase.Tint = Tint;

    if (!Enabled) return;

    timeSinceLastHarvest += deltaTime;
    if (timeSinceLastHarvest >= harvestInterval) {
        timeSinceLastHarvest -= harvestInterval;

        PlayScene* scene = getPlayScene();
        scene->EarnMoney(moneyPerHarvest);

        // Optional: sound effect
        AudioHelper::PlayAudio("gold.wav");
    }
}
