#ifndef SOLDIERENEMY_HPP
#define SOLDIERENEMY_HPP
#include "Enemy.hpp"

class SoldierEnemy : public Enemy {
public:
    SoldierEnemy(int x, int y);
    void Draw() const override;
    void Update(float deltaTime) override;
    void Hit(float damage) override;
};
#endif   // SOLDIERENEMY_HPP
