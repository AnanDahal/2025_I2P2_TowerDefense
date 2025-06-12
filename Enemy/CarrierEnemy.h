#ifndef CARRIERENEMY_H
#define CARRIERENEMY_H
#include "Enemy.hpp"

class CarrierEnemy : public Enemy {
public:
    CarrierEnemy(int x, int y);
protected:
    void OnExplode() override;
};
#endif //CARRIERENEMY_H