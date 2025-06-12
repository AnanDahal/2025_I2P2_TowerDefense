#include "EnemyBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/Collider.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"
#include "MissEnemyBullet.h"

MissEnemyBullet::MissEnemyBullet(Engine::Point pos,Engine::Point dir,float rot, float speed, float damage)
  : EnemyBullet("play/bullet-8.png", pos, dir, rot, speed, damage)
{
    Velocity        = dir.Normalize() * speed;
    Rotation        = rot;
    CollisionRadius = 4;
}
