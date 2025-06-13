//
// Created by user on 6/12/2025.
//

#ifndef SHOPSCENE_H
#define SHOPSCENE_H
#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class ShopScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bellInstance;
public:
    explicit ShopScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void BackOnClick(int stage);
    void ChooseOnClick(int skin);
};

#endif //SHOPSCENE_H
