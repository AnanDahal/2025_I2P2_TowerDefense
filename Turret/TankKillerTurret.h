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
    static bool isLocked;
};
#endif //TANKKILLERTURRET_H
