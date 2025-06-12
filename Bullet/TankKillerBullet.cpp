//
// Created by user on 5/16/2025.
//
#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "TankKillerBullet.h"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

class Turret;

TankKillerBullet::TankKillerBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, bool buff, bool miss) : Bullet("play/bullet-7.png", 900, 2, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent, buff, false, miss) {
}
void TankKillerBullet::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), enemy->Position.x, enemy->Position.y));
    if (enemy->type == 1) enemy->Hit(damage, false); //double damage to tanks
}