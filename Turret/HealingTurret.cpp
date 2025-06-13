// HealingTurret.cpp  :contentReference[oaicite:0]{index=0}
#include <cmath>
#include "HealingTurret.hpp"
#include "Scene/PlayScene.hpp"
#include "Bullet/HealingBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Scene/PasswordScene.h"

// 1) Define the static Price exactly like the others:
const int HealingTurret::Price = 200;
// 2) Define the ctor, forwarding to Turret’s base-ctor in the same style:
HealingTurret::HealingTurret(float x, float y)
    : Turret(
        "play/tower-base.png",   // base sprite
        "play/turret-3.png",     // turret head sprite
         x, y,                   // position
         200,                    // range
         Price,                  // cost
         1.0f,                   // fire rate (seconds per shot)
         200                     // hp
      )
{
    // keep the same “center adjustment” pattern your other turrets use
    Anchor.y += 8.0f / GetBitmapHeight();
    isLocked = false;
}

void HealingTurret::Update(float deltaTime) {
    // 1) Base sprite + base image sync
    if (healUpgrade2) {
        coolDown /= 2;
    }
    Sprite::Update(deltaTime);
    imgBase.Position = Position;
    imgBase.Tint     = Tint;
    if (!Enabled) return;

    // 2) Handle reload
    reload -= deltaTime;
    if (reload > 0) return;
    reload += coolDown;

    // 3) Find the best turret to heal
    Turret* target = FindTargetTurret();
    if (!target) return;

    // 4) Aim at it
    auto diff = target->Position - Position;
    auto dir  = diff.Normalize();
    Rotation   = atan2(dir.y, dir.x) + ALLEGRO_PI/2;

    // 5) Fire healing bullet
    Engine::Point spawnPos = Position + dir * 36;
    getPlayScene()->BulletGroup->AddNewObject(
        new HealingBullet(spawnPos, dir, Rotation - ALLEGRO_PI/2, this, buffed, healUpgrade1)
    );
    AudioHelper::PlayAudio("gun.wav");
}

Turret* HealingTurret::FindTargetTurret() {
    PlayScene* scene = getPlayScene();
    Turret* best     = nullptr;
    float bestDist   = CollisionRadius;

    for (auto& it : scene->TowerGroup->GetObjects()) {
        Turret* t = dynamic_cast<Turret*>(it);
        if (!t || !t->Enabled) continue;
        // skip itself and other healing turrets
        if (t == this || dynamic_cast<HealingTurret*>(t)) continue;
        // only damaged ones
        if (t->GetCurrentHp() >= t->GetMaxHp()) continue;

        float d = (t->Position - Position).Magnitude();
        if (d <= bestDist && (!best || d < bestDist)) {
            best     = t;
            bestDist = d;
        }
    }
    return best;
}


void HealingTurret::CreateBullet() {
    // empty or you can remove it entirely
}
