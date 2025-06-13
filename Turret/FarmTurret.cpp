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

const int FarmTurret::Price = 400;
FarmTurret::FarmTurret(float x, float y) : Turret("play/tower-base.png", "play/turret-10.png", x, y, 300, Price, 1, 600) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
    if (OnStage == 3 || OnStage == 4) {
        isLocked = false;
    }
    else {
        isLocked = true;
    }
}

void FarmTurret::CreateBullet() {
}

void FarmTurret::Update(float deltaTime) {
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
