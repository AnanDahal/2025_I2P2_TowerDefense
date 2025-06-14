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
    float coolDown;
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
    inline std::string GetTurretImage(int turretNum) {
    extern bool skin1;
    extern bool skin2;
    if (skin2)
        return "play/Turret upgrades/turret-" + std::to_string(turretNum) + "-u2.png";
    else if (skin1)
        return "play/Turret upgrades/turret-" + std::to_string(turretNum) + "-u1.png";
    else
        return "play/turret-" + std::to_string(turretNum) + ".png";
    }   

};
#endif   // TURRET_HPP