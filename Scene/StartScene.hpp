//
// Created by Hsuan on 2024/4/10.
//

#ifndef INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
#define INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
extern int OnStage;
extern int core_memories;
extern int endless_score;
extern bool skin1;
extern bool skin2;
extern bool skin3;
extern bool skin4;

class StartScene final : public Engine::IScene {
    int fadeticks;
    int timer;
    int fadealpha;
    int curX = 0;
    int alpha = 0;
    ALLEGRO_BITMAP *house;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> cricketInstance;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sneakInstance;
public:
    explicit StartScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void Update(float deltaTime) override;
    void PlayOnClick(int stage);
    void SettingsOnClick(int stage);
};
#endif   // INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
