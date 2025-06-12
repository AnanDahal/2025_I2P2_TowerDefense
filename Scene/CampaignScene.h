//
// Created by Farrel on 6/12/2025.
//

#ifndef CAMPAIGNSCENE_H
#define CAMPAIGNSCENE_H
#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class CampaignScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit CampaignScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void BackOnClick(int stage);
};

#endif //CAMPAIGNSCENE_H
