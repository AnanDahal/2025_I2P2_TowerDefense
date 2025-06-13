#ifndef MACHINEGUNTURRET_HPP
#define MACHINEGUNTURRET_HPP
#include "Turret.hpp"

class MachineGunTurret : public Turret {
public:
    static const int Price;
    MachineGunTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    static bool machinegunupgrade1;
    static bool machinegunupgrade2;
};

#endif   // MACHINEGUNTURRET_HPP
