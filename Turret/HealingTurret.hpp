// HealingTurret.hpp

#ifndef HEALINGTURRET_HPP
#define HEALINGTURRET_HPP
#include "Turret.hpp"
#include "Engine/Sprite.hpp"

class HealingTurret : public Turret {
public:
    static const int Price;
    HealingTurret(float x, float y);
    static bool isLocked;
    // Override base targeting/shooting entirely
    void Update(float deltaTime) override;
    void CreateBullet() override;
    static bool healUpgrade1;
    static bool healUpgrade2;
private:
    Turret* FindTargetTurret();
};

#endif   // HEALINGTURRET_HPP
