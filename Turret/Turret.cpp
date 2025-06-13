#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <algorithm>    // for std::max
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret.hpp"
#include "Scene/PlayScene.hpp"
#include <random>
#include "UI/Animation/ExplosionEffect.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Engine/AudioHelper.hpp"

PlayScene *Turret::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

// ── NEW: accept an hp parameter ─────────────────
Turret::Turret(std::string imgBase,
               std::string imgTurret,
               float x, float y,
               float radius,
               int price,
               float coolDown,
               int hp)
  : Sprite(imgTurret, x, y),
    price(price),
    coolDown(coolDown),
    imgBase(imgBase, x, y),
    maxHp(hp),
    currentHp(hp)
{
    CollisionRadius = radius;
}
// ────────────────────────────────────────────────

void Turret::Update(float deltaTime) {
    // 1) Base sprite update + keep the base image in sync
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();
    imgBase.Position = Position;
    imgBase.Tint     = Tint;

    // 2) If we’re “dead”, skip all targeting/shooting
    if (!Enabled) return;

    // 3) If we already have a target, but it’s no longer valid, unlock it
    if (Target) {
        Engine::Point diff = Target->Position - Position;
        if (!Target->IsTargetable() || diff.Magnitude() > CollisionRadius) {
            Target->lockedTurrets.erase(lockedTurretIterator);
            Target = nullptr;
            lockedTurretIterator = std::list<Turret *>::iterator();
        }
    }

    // 4) Acquire a new target if we don’t have one yet
    if (!Target) {
        for (auto &it : scene->EnemyGroup->GetObjects()) {
            Enemy *e = dynamic_cast<Enemy *>(it);
            if (!e || !e->IsTargetable()) continue;
            Engine::Point diff = it->Position - Position;
            if (diff.Magnitude() <= CollisionRadius) {
                Target = e;
                Target->lockedTurrets.push_back(this);
                lockedTurretIterator = std::prev(Target->lockedTurrets.end());
                break;
            }
        }
    }

    // 5) If we have a valid target now, rotate toward it and fire when reloaded
    if (Target) {
        // a) Compute “forward” vector from current Rotation
        Engine::Point originRotation(
            cos(Rotation - ALLEGRO_PI / 2),
            sin(Rotation - ALLEGRO_PI / 2)
        );
        // b) Compute desired aim vector
        Engine::Point targetRotation = (Target->Position - Position).Normalize();
        // c) Clamp turning speed
        float maxRotateRadian = rotateRadian * deltaTime;
        float cosTheta = originRotation.Dot(targetRotation);
        cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);
        float radian = acos(cosTheta);
        Engine::Point newDir;
        if (std::abs(radian) <= maxRotateRadian)
            newDir = targetRotation;
        else
            newDir = ((std::abs(radian) - maxRotateRadian) * originRotation
                      + maxRotateRadian * targetRotation) / radian;
        // d) Apply rotation (add 90° because the sprite points up)
        Rotation = atan2(newDir.y, newDir.x) + ALLEGRO_PI / 2;

        // e) Handle reload & shoot
        reload -= deltaTime;
        if (reload <= 0) {
            reload = coolDown;
            CreateBullet();
        }
    }
}


void Turret::Draw() const {
    if (Preview) {
        al_draw_filled_circle(Position.x, Position.y, CollisionRadius,
                              al_map_rgba(0, 255, 0, 50));
    }

    imgBase.Draw();
    Sprite::Draw();

    // ── NEW: draw HP bar ───────────────────────────
    const float barW = 40;
    const float barH = 6;
    // vertical offset: sit it just above the sprite top
    float topY = Position.y - GetBitmapHeight() * Anchor.y - barH - 4;
    float leftX = Position.x - barW/2;

    // black background
    al_draw_filled_rectangle(leftX, topY,
                             leftX + barW, topY + barH,
                             al_map_rgb(0, 0, 0));
    // health fraction
    float frac = float(currentHp) / float(maxHp);
    // green→red gradient
    int r = (int)((1 - frac) * 255);
    int g = (int)(frac * 255);
    al_draw_filled_rectangle(leftX + 1, topY + 1,
                             leftX + 1 + (barW - 2) * frac,
                             topY + barH - 1,
                             al_map_rgb(r, g, 0));
    // border
    al_draw_rectangle(leftX, topY,
                      leftX + barW, topY + barH,
                      al_map_rgb(0, 0, 0), 1);
    // ────────────────────────────────────────────────

    if (PlayScene::DebugMode) {
        // Draw target radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius,
                       al_map_rgb(0, 0, 255), 2);
    }
}

void Turret::OnDestroyed() {
    auto scene = getPlayScene();

    // 1) Explosion effect + dirty debris (same as Enemy::OnExplode)
    scene->EffectGroup->AddNewObject(
        new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        scene->GroundEffectGroup->AddNewObject(
            new DirtyEffect(
                "play/dirty-" + std::to_string(distId(rng)) + ".png",
                dist(rng),
                Position.x, Position.y));
    }  // :contentReference[oaicite:0]{index=0}

    // 2) Un‐lock from any enemy that was targeting this turret
    if (Target) {
        Target->lockedTurrets.erase(lockedTurretIterator);
        Target = nullptr;
    }

    // 3) Remove this turret from the play‐scene
    scene->TowerGroup->RemoveObject(objectIterator);

    // 4) Play destruction sound
    AudioHelper::PlayAudio("explosion.wav");  // :contentReference[oaicite:1]{index=1}
    // NEW: frees cell for build but not for enemy pathing
    int cellX = static_cast<int>(Position.x / PlayScene::BlockSize);
    int cellY = static_cast<int>(Position.y / PlayScene::BlockSize);
    scene->FreeTile(cellX, cellY);
}


int Turret::GetPrice() const {
    return price;
}