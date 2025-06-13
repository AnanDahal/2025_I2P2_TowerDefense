#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

extern int core_memories;

class Bullet;
class PlayScene;
class Turret;

class Enemy : public Engine::Sprite {
protected:
    std::vector<Engine::Point> path;
    float speed;
    float hp;
    float refhp;
    int money;
    PlayScene *getPlayScene();
    virtual void OnExplode();
    bool _invisible = false;
    float _invisibleTime = 0;
    float _invisibleDuration = 0;

public:
    float reachEndTime;
    int type;
    std::list<Turret *> lockedTurrets;
    std::list<Bullet *> lockedBullets;
    Enemy(std::string img, int type, float x, float y, float radius, float speed, float hp, int money);
    virtual void Hit(float damage, bool slow);
    void UpdatePath(const std::vector<std::vector<int>> &mapDistance);
    void Update(float deltaTime) override;
    void Draw() const override;
    virtual bool IsTargetable() const {return !_invisible;}

    void SetInvisible(float duration) {
        _invisible = true;
        _invisibleTime = 0.0f;
        _invisibleDuration = duration;
    }
};
#endif   // ENEMY_HPP
