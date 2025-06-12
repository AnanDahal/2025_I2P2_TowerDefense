#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "SniperBullet.h"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

class Turret;

SniperBullet::SniperBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff, bool miss) : Bullet("play/bullet-9.png", 1000, 5, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent, buff, false, miss) {
}
void SniperBullet::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-1.png", dist(rng), enemy->Position.x, enemy->Position.y));
    if (enemy->type == 4) enemy->Hit(damage, false); //double damage to bosses
}
float SniperBullet::getSpeed() {
    return 1000;
};