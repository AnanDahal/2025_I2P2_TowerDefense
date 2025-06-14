//
// Created by Farrel on 6/14/2025.
//

#ifndef CLOSINGSCENE_H
#define CLOSINGSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class ClosingScene final : public Engine::IScene {
    int fadeticks;
    int timer;
    int fadealpha;
    
    // Add variables for finalscene animation
    int finalSceneX;
    int finalSceneY;
    
    // Add variables for weird scene
    bool showWeirdScene = false;
    int weirdSceneTimer = 0;
    int weirdSceneAlpha = 0;
    bool weirdSceneDissolveIn = true;
    
    // Add variables for panda fade and weird screen
    bool showPandaFade = false;
    int pandaFadeFrame = 0;
    int pandaFadeAlpha = 0;
    int pandaFadeTimer = 0;
    bool showWeirdScreen = false;
    int weirdScreenAlpha = 0;
    
    // Final fade to black
    bool finalFadeOut = false;
    int finalFadeTimer = 0;
    int finalFadeAlpha = 0;

    ALLEGRO_BITMAP *finalScene;
    ALLEGRO_BITMAP *weirdScene;
    ALLEGRO_BITMAP *weirdScreen;
    ALLEGRO_BITMAP *pandaFade;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> cricketInstance;

public:
    int changeid;  // Default story ID
    explicit ClosingScene() = default;
    void Initialize() override;
    void Draw() const override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keycode);
};

#endif //CLOSINGSCENE_H
