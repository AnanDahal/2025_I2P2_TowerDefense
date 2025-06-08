#include "EnemyBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/Collider.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"

PlayScene *EnemyBullet::getPlayScene() {
    return dynamic_cast<PlayScene *>(
        Engine::GameEngine::GetInstance().GetActiveScene()
    );
}

EnemyBullet::EnemyBullet(const std::string &sprite,
                         Engine::Point pos,
                         Engine::Point dir,
                         float rot,
                         float speed,
                         float damage)
  : Sprite(sprite, pos.x, pos.y),  // ← now uses your passed-in sprite
    speed(speed), damage(damage)
{
    Velocity        = dir.Normalize() * speed;
    Rotation        = rot;
    CollisionRadius = 4;
}

void EnemyBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    auto scene = getPlayScene();

    // 1) Damage any turret we actually touch (not its targeting circle)
    for (auto &obj : scene->TowerGroup->GetObjects()) {
        Turret *t = dynamic_cast<Turret *>(obj);
        if (!t || !t->Enabled) continue;

        // Use sprite size, not the 200/300px target radius:
        float tw = static_cast<float>(t->GetBitmapWidth());
        float th = static_cast<float>(t->GetBitmapHeight());
        float spriteRadius = std::max(tw, th) * 0.5f;

        if (Engine::Collider::IsCircleOverlap(
                Position,           // bullet center
                CollisionRadius,    // bullet radius
                t->Position,        // turret center
                spriteRadius))      // turret graphic radius
        {
            t->TakeDamage(damage);
            scene->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }

    // 2) Remove if really off‐screen (optional)
    auto screen = PlayScene::GetClientSize();
    if (!Engine::Collider::IsRectOverlap(
            Position - Size/2,
            Position + Size/2,
            Engine::Point(0,0),
            screen))
    {
        scene->BulletGroup->RemoveObject(objectIterator);
    }
}
