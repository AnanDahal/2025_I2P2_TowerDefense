#include "HealingBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "Engine/Collider.hpp"

HealingBullet::HealingBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff)
    : Bullet("play/bullet-7.png", 0, 0, position, forwardDirection, rotation, parent, buff, false) {
    speed = 300;  // Ensure bullet speed is set
    damage = 0;   // No damage, just healing
    // Set velocity to move the bullet forward
    Velocity = forwardDirection * speed;
}

void HealingBullet::OnExplode(Turret *turret) {
    if (turret == nullptr) return;

    // Heal the turret by a fixed amount (e.g., 10 HP)
    int newHp = turret->GetCurrentHp() + 50;
    turret->SetCurrentHp(newHp);

    // Prevent exceeding max HP
    if (turret->GetCurrentHp() > turret->GetMaxHp()) {
        turret->SetCurrentHp(turret->GetMaxHp());
    }

}

void HealingBullet::Update(float deltaTime) {
    // Move the bullet forward based on its velocity
    // In HealingBullet::Update() or wherever you're updating Position
    Position.x += Velocity.x * deltaTime;
    Position.y += Velocity.y * deltaTime;


    // Check for collision with turrets and trigger healing
    PlayScene *scene = getPlayScene();
    for (auto &it : scene->TowerGroup->GetObjects()) {
        Turret *t = dynamic_cast<Turret *>(it);
        if (!t || !t->Enabled) continue;

        // Check if the turret is within range
        float tw = static_cast<float>(t->GetBitmapWidth());
        float th = static_cast<float>(t->GetBitmapHeight());
        float spriteRadius = std::max(tw, th) * 0.5f;

        // If the bullet collides with the turret, heal it
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, t->Position, spriteRadius)) {
            OnExplode(t);  // Heal the turret
            scene->BulletGroup->RemoveObject(objectIterator);  // Remove the healing bullet
            return;  // Exit after healing the first turret
        }
    }

    // Check if the bullet is out of bounds
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize())) {
        scene->BulletGroup->RemoveObject(objectIterator);  // Remove the bullet if out of bounds
    }
}
