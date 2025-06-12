//
// Created by Farrel on 6/10/2025.
//

#ifndef OPENINGSCENE_H
#define OPENINGSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class OpeningScene final : public Engine::IScene {
    int fadeticks;
    int tick;
    int timer;
    int curX;
    int curY;
    int charframe;
    float charx;
    int fadealpha;
    int alpha;

    bool done;
    bool show;
    bool played;
    ALLEGRO_BITMAP *house;
    ALLEGRO_BITMAP *chara;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> cricketInstance;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sneakInstance;

public:
    explicit OpeningScene() = default;
    void Initialize() override;
    void Draw() const override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keycode);
};

#endif //OPENINGSCENE_H
