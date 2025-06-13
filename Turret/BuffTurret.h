//
// Created by user on 5/18/2025.
//

#ifndef BUFFTURRET_H
#define BUFFTURRET_H
#include "Turret.hpp"

class BuffTurret : public Turret {
public:
    static const int Price;
    BuffTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool isLocked;
    static bool buffUpgrade1;
    static bool buffUpgrade2;
};

#endif //BUFFTURRET_H
