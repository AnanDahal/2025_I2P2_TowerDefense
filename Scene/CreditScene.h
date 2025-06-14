//
// Created by user on 6/14/2025.
//

#ifndef CREDITSCENE_H
#define CREDITSCENE_H

#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class CreditScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit CreditScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void BackOnClick(int stage);
};

#endif //CREDITSCENE_H
