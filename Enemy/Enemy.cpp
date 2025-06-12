#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Bullet/Bullet.hpp"
#include "Enemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

PlayScene *Enemy::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Enemy::OnExplode() {
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}
Enemy::Enemy(std::string img, int type,float x, float y, float radius, float speed, float hp, int money) : Engine::Sprite(img, x, y), type(type), speed(speed), hp(hp), money(money) {
    CollisionRadius = radius;
    reachEndTime = 0;
    refhp = hp;
}
void Enemy::Hit(float damage, bool slow) {
    hp -= damage;
    if (slow && speed <= 5) {
        speed -= 5;
    }
    if (hp <= 0) {
        OnExplode();
        // Remove all turret's reference to target.
        for (auto &it : lockedTurrets)
            it->Target = nullptr;
        for (auto &it : lockedBullets)
            it->Target = nullptr;
        getPlayScene()->EarnMoney(money);
        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }
}
void Enemy::UpdatePath(const std::vector<std::vector<int>> &mapDistance) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));

    // If outside the map, move to the nearest valid tile inside the map
    int startX = x, startY = y;
    if (x < 0) startX = 0;
    if (x >= PlayScene::MapWidth) startX = PlayScene::MapWidth - 1;
    if (y < 0) startY = 0;
    if (y >= PlayScene::MapHeight) startY = PlayScene::MapHeight - 1;

    // Only proceed if the starting tile is a path tile
    if (mapDistance[startY][startX] == -1) {
        Engine::LOG(Engine::ERROR) << "Enemy path finding error";
        path.clear();
        return;
    }

    int num = mapDistance[startY][startX];
    path = std::vector<Engine::Point>(num + 1);
    Engine::Point pos(startX, startY);
    while (num != 0) {
        std::vector<Engine::Point> nextHops;
        for (auto &dir : PlayScene::directions) {
            int nx = pos.x + dir.x;
            int ny = pos.y + dir.y;
            if (nx < 0 || nx >= PlayScene::MapWidth || ny < 0 || ny >= PlayScene::MapHeight)
                continue;
            if (mapDistance[ny][nx] != num - 1)
                continue;
            nextHops.emplace_back(nx, ny);
        }
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
        pos = nextHops[dist(rng)];
        path[num] = pos;
        num--;
    }
    path[0] = PlayScene::EndGridPoint;
}


void Enemy::Update(float deltaTime) {
    if (_invisible) {
        _invisibleTime += deltaTime;
        if (_invisibleTime >= _invisibleDuration) {
            _invisible = false;
        }
    }
    // Pre-calculate the velocity.
    float remainSpeed = speed * deltaTime;
    if (type != 0 && type % 2 == 0) remainSpeed *= (refhp - hp <= 1) ? 1 : (refhp - hp) / 2;
    while (remainSpeed != 0) {
        if (path.empty()) {
            // Reach end point.
            Hit(hp, false);
            getPlayScene()->Hit();
            reachEndTime = 0;
            return;
        }
        Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        // Add up the distances:
        // 1. to path.back()
        // 2. path.back() to border
        // 3. All intermediate block size
        // 4. to end point
        reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
        Engine::Point normalized = vec.Normalize();
        if (remainSpeed - vec.Magnitude() > 0) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        } else {
            Velocity = normalized * remainSpeed / deltaTime;
            remainSpeed = 0;
        }
    }
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
}
void Enemy::Draw() const {
    if (_invisible) return;
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}
