//
// Created by Farrel on 6/12/2025.
//

#ifndef AOETURRET_H
#define AOETURRET_H
#include "Turret.hpp"

class MissileTurret : public Turret {
public:
    static const int Price;
    MissileTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool missileUpgrade1;
    static bool missileUpgrade2;
};

#endif //AOETURRET_H
