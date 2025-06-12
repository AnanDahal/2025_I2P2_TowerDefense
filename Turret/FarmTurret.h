//
// Created by user on 6/13/2025.
//

#ifndef FARMTURRET_H
#define FARMTURRET_H
#include "Turret.hpp"

class FarmTurret : public Turret {
public:
    static const int Price;
    FarmTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool isLocked;
    float timeSinceLastHarvest = 0.0f;
    const float harvestInterval = 2.0f; // seconds
    const int moneyPerHarvest = 10;
};
#endif //FARMTURRET_H
