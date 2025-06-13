//
// Created by user on 6/12/2025.
//

#ifndef TANKKILLERTURRET_H
#define TANKKILLERTURRET_H
#include "Turret.hpp"
class TankKillerTurret : public Turret {
public:
    static const int Price;
    TankKillerTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool isLocked;
    static bool tankUpgrade1;
    static bool tankUpgrade2;
};

#endif //TANKKILLERTURRET_H
