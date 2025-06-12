//
// Created by user on 6/10/2025.
//

#ifndef LOGINSCENE_H
#define LOGINSCENE_H
class LogInScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    void SubmitName();
    float ticks;

    Engine::Label* nameLabel = nullptr;

public:

    explicit LogInScene() = default;
    void Initialize() override;
    void Terminate() override;
    //void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void OnKeyDown(int keyCode) override;
    void OnCharPress(int unicode);
    void Draw ()const;
    static std::string getPlayerName();
};
static std::string playerName;
#endif //LOGINSCENE_H

