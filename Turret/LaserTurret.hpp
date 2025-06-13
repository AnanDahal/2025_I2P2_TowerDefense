#ifndef LASERTURRET_HPP
#define LASERTURRET_HPP
#include "Turret.hpp"

class LaserTurret : public Turret {
public:
    static const int Price;
    LaserTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool laserUpgrade1;
    static bool laserUpgrade2;
};

#endif   // LASERTURRET_HPP
