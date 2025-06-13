//
// Created by Farrel on 6/12/2025.
//
#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "MissileBullet.h"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

MissileBullet::MissileBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent, float aoeRadius,bool buff, bool miss, bool upgrade) : AOEBullet("play/bullet-3.png", 600, 3, position, forwardDirection, rotation - ALLEGRO_PI * 3/ 2, parent, aoeRadius, buff, false, miss, upgrade) {
}
void MissileBullet::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-1.png", dist(rng), enemy->Position.x, enemy->Position.y));
}