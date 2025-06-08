// HealingTurret.hpp

#ifndef HEALINGTURRET_HPP
#define HEALINGTURRET_HPP
#include "Turret.hpp"
#include "Engine/Sprite.hpp"

class HealingTurret : public Turret {
public:
    static const int Price;
    HealingTurret(float x, float y);

    // Override base targeting/shooting entirely
    void Update(float deltaTime) override;
    void CreateBullet() override;
private:
    Turret* FindTargetTurret();
};
#endif   // HEALINGTURRET_HPP
