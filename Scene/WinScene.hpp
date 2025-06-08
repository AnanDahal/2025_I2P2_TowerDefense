#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class WinScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    std::string playerName;  // Add this to store the current input
    Engine::Label* nameLabel;  // Add this to show the current input


public:
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
};

#endif   // WINSCENE_HPP