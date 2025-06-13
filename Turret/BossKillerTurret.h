//
// Created by user on 6/12/2025.
//

#ifndef BOSSKILLERTURRET_H
#define BOSSKILLERTURRET_H
#include "Turret.hpp"
class BossKillerTurret : public Turret {
public:
    static const int Price;
    BossKillerTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool isLocked;
    static bool bossUpgrade1;
    static bool bossUpgrade2;
};

#endif //BOSSKILLERTURRET_H
