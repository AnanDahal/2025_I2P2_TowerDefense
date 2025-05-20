//
// Created by Lenovo on 5/17/2025.
//

#ifndef LEADERBOARDSCENE_H
#define LEADERBOARDSCENE_H

#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class LeaderBoardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit LeaderBoardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    // void Draw() const override;
};


#endif //LEADERBOARDSCENE_H