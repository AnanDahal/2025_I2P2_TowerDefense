// HealingBullet.hpp
#ifndef HEALINGBULLET_HPP
#define HEALINGBULLET_HPP
#include "Bullet.hpp"

class Turret;

class HealingBullet : public Bullet {
public:
    explicit HealingBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buffed, bool upgrade);
    void OnExplode(Turret *turret);  // Healing explosion
    void Update(float deltaTime) override;
};
#endif  // HEALINGBULLET_HPP
