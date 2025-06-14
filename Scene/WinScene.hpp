#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
extern int OnStage;
extern int core_memories;
extern int endless_score;
extern bool skin1;
extern bool skin2;

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
    int winstage = 7;
};

#endif   // WINSCENE_HPP