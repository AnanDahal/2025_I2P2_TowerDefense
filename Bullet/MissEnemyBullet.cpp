#include "EnemyBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/Collider.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"
#include "MissEnemyBullet.h"

// PlayScene *EnemyBullet::getPlayScene() {
//     return dynamic_cast<PlayScene *>(
//         Engine::GameEngine::GetInstance().GetActiveScene()
//     );
// }

MissEnemyBullet::MissEnemyBullet(Engine::Point pos,Engine::Point dir,float rot)
  : EnemyBullet("play/bullet-8.png", pos, dir, rot, 400.0f, 0)
{
    Velocity        = dir.Normalize() * speed;
    Rotation        = rot;
    CollisionRadius = 4;
}
