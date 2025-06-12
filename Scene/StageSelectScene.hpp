#ifndef STAGESELECTSCENE_HPP
#define STAGESELECTSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
#include "UI/Component/ImageButton.hpp"

class StageSelectScene final : public Engine::IScene {
private:
    int ticks, timers;
    int fadealpha;
    int bgX, dbX;
    bool fadeout;
    int changeto, stagenum;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> snoreInstance;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> violetInstance;
    ALLEGRO_BITMAP *bg;
    ALLEGRO_BITMAP *db;
public:
    explicit StageSelectScene() = default;
    void Initialize() override;
    void Draw() const override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void PlayOnClick(int stage);
    void ScoreboardOnClick(int stage);
    void BackOnClick(int stage);
    void PowersOnClick(int stage);

    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif   // STAGESELECTSCENE_HPP
