//
// Created by user on 6/11/2025.
//

#ifndef PASSWORDSCENE_H
#define PASSWORDSCENE_H

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
static int OnStage;
static int core_memories;
static int endless_score;
#endif //PASSWORDSCENE_H
