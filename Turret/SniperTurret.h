//
// Created by user on 5/25/2025.
//

#ifndef SNIPERTURRET_H
#define SNIPERTURRET_H
#include "Turret.hpp"
class SniperTurret : public Turret {
public:
    static const int Price;
    SniperTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool isLocked;
    static bool sniperUpgrade1;
    static bool sniperUpgrade2;
};

#endif //SNIPERTURRET_H
