//
// Created by user on 6/12/2025.
//

#ifndef POWERS_H
#define POWERS_H
#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
extern int whichPower;
class PowersScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit PowersScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void BackOnClick(int stage);
    void ChooseOnClick(int power);
};


#endif //POWERS_H
