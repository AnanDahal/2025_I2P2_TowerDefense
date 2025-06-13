//
// Created by user on 6/11/2025.
//

#ifndef PASSWORDSCENE_H
#define PASSWORDSCENE_H
#include <memory>                          // For shared_ptr
#include <allegro5/allegro_audio.h>        // For ALLEGRO_SAMPLE_INSTANCE
#include <string>                          // For std::string
#include "Engine/IScene.hpp"               // Include your IScene base class
#include "UI/Component/Label.hpp"          // Include the Label class
extern int OnStage;
extern int core_memories;
extern int endless_score;
extern bool skin1;
extern bool skin2;
extern bool skin3;
extern bool skin4;

class PasswordScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    void SubmitName();
    float ticks;
    //std::string playerName;
    std::string playerPassword;
    Engine::Label* nameLabel = nullptr;

public:
    explicit PasswordScene() = default;
    void Initialize() override;
    void Terminate() override;
    //void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void OnKeyDown(int keyCode) override;
    void OnCharPress(int unicode);
    void Draw ()const;
    //void SetPlayerName(std::string name);

};

#endif //PASSWORDSCENE_H
