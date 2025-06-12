#include <allegro5/base.h>
#include <random>
#include <cmath>
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Bullet/EnemyBullet.hpp"
#include "Turret/Turret.hpp"
#include "TankEnemy.hpp"

TankEnemy::TankEnemy(int x, int y)
    : Enemy("play/enemy-3.png", 0,x, y, 20.f, 20.f, 100.f, 50),
      head("play/enemy-3-head.png", x, y),
      targetRotation(0.0f) {
}

void TankEnemy::Draw() const {
    Enemy::Draw();
    head.Draw();
}

void TankEnemy::Update(float deltaTime) {
    // 1) movement etc.
    Enemy::Update(deltaTime);

    // 2) aim (or wobble if no turret)
    auto scene = dynamic_cast<PlayScene*>(
        Engine::GameEngine::GetInstance().GetActiveScene()
    );
    Turret* best = nullptr;
    float   bestDist = shootRadius;
    for (auto obj : scene->TowerGroup->GetObjects()) {
        if (auto t = dynamic_cast<Turret*>(obj); t && t->Enabled) {
            float d = (t->Position - Position).Magnitude();
            if (d < bestDist) { bestDist = d; best = t; }
        }
    }

    if (best) {
        auto dir = (best->Position - Position).Normalize();
        head.Rotation = atan2(dir.y, dir.x);
    } else {
        // corrected wobble
        head.Position = Position;
        static std::random_device dev;
        static std::mt19937        rng(dev());
        std::uniform_real_distribution<float> pick(0.0f, 4.0f);
        if (pick(rng) < deltaTime) {
            std::uniform_real_distribution<float> ang(-ALLEGRO_PI, ALLEGRO_PI);
            targetRotation = ang(rng);
        }
        head.Rotation = (head.Rotation + deltaTime * targetRotation)
                        / (1.0f + deltaTime);
    }
    head.Position = Position;

    // 3) cooldown & fire
    shootReload -= deltaTime;
    if (best && shootReload <= 0.0f) {
        auto dir = (best->Position - Position).Normalize();
        float rot = atan2(dir.y, dir.x);
        scene->BulletGroup->AddNewObject(
            new EnemyBullet(
                "play/bullet-9.png", // image
                Position,    // start
                dir,         // direction
                rot,         // rotation
                shootSpeed,  // speed
                shootDamage  // damage
            )
        );
        AudioHelper::PlayAudio("gun.wav");
        shootReload = shootCooldown;
    }
}
