#ifndef CARRIERENEMY_H
#define CARRIERENEMY_H
#include "Enemy.hpp"

class CarrierEnemy : public Enemy {
public:
    CarrierEnemy(int x, int y, bool fast);
protected:
    void OnExplode() override;
};
#endif //CARRIERENEMY_H