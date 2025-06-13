//
// Created by Farrel on 6/12/2025.
//

#include "AOEBullet.h"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"
#include <algorithm>

PlayScene *AOEBullet::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void AOEBullet::OnExplode(Enemy *enemy) {
}
AOEBullet::AOEBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, float aoeRadius, bool buff, bool slow, bool miss, bool upgrade) : Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent), aoeRadius(aoeRadius) {
    Velocity = forwardDirection.Normalize() * speed;
    Rotation = rotation;
    CollisionRadius = 6;
    isBuffed = buff;
    isSlow = slow;
    isMiss = miss;
    isUpgraded = upgrade;
}
void AOEBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();
    if (isUpgraded) {
        aoeRadius *= 2;
    }
    bool hitEnemy = false;
    std::vector<Enemy*> hitEnemies;

    // Check collision with all enemies
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(it);
        if (!enemy->Visible)
            continue;

        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius,
                                            enemy->Position, enemy->CollisionRadius)) {
            hitEnemy = true;
            hitEnemies.push_back(enemy);
        }
    }

    // If we hit at least one enemy directly (with the bullet's collision radius)
    if (hitEnemy) {
        // Apply AoE damage to all enemies within the larger radius
        if (aoeRadius > 0) {
            for (auto& it : scene->EnemyGroup->GetObjects()) {
                Enemy* enemy = dynamic_cast<Enemy*>(it);
                if (!enemy->Visible)
                    continue;

                // Check if enemy is within AoE radius
                if (Engine::Collider::IsCircleOverlap(Position, aoeRadius, enemy->Position, enemy->CollisionRadius)) {
                    // Make sure we haven't already hit this enemy directly
                    if (std::find(hitEnemies.begin(), hitEnemies.end(), enemy) == hitEnemies.end()) {
                        if (isBuffed) {
                            if (isMiss) enemy->Hit(0, false);
                            else enemy->Hit(2*damage, isSlow);
                        }
                        else {
                            if (isMiss) enemy->Hit(0, false);
                            else enemy->Hit(damage, isSlow);
                        }
                    }
                }
            }
        }

        // Damage all directly hit enemies
        for (Enemy* enemy : hitEnemies) {
            OnExplode(enemy);
            if (isBuffed) {
                if (isMiss) enemy->Hit(0, false);
                else enemy->Hit(2*damage, isSlow);
            }
            else {
                if (isMiss) enemy->Hit(0, false);
                else enemy->Hit(damage, isSlow);
            }
        }

        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }

    // Check if out of boundary
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize())) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
    }
}