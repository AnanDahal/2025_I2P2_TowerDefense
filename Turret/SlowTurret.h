//
// Created by user on 6/10/2025.
//

#ifndef SLOWTURRET_H
#define SLOWTURRET_H
#include "Turret.hpp"

class SlowTurret : public Turret {
public:
    static const int Price;
    SlowTurret(float x, float y);
    void CreateBullet() override;
    static bool isLocked;
};

#endif //SLOWTURRET_H
