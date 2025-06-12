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
    static bool isLocked;
};
#endif //AOETURRET_H
