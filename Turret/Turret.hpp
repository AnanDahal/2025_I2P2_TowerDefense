#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;

class Turret : public Engine::Sprite {
protected:
    int price;
    int currentHp;
    int maxHp;
    float coolDown;
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;
    Sprite imgBase;
    std::list<Turret *>::iterator lockedTurretIterator;
    PlayScene *getPlayScene();
    // Reference: Design Patterns - Factory Method.
    virtual void CreateBullet() = 0;
    virtual void OnDestroyed();

public:
    bool Enabled = true;
    bool Preview = false;
    bool buffed = false;
    bool missing = false;
    Enemy *Target = nullptr;
    Turret(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price, float coolDown, int hp);
    void Update(float deltaTime) override;
    void Draw() const override;
    int GetPrice() const;
    int GetCurrentHp() const { return currentHp; }
    void SetCurrentHp(int hp) {
        currentHp = hp;
        if (currentHp > maxHp) currentHp = maxHp;  // Ensure it doesn't exceed maxHp
    }
    int GetMaxHp() const { return maxHp; }
    int missThreshold;

    void TakeDamage(int damage) {
        if (damage == 0) {
            //boolean for missing
            missing = true;
            missThreshold = 0;
        }
        currentHp -= damage;
        if (currentHp <= 0) {
            currentHp = 0;
            Enabled = false;
            OnDestroyed();
        }
    }

};
#endif   // TURRET_HPP