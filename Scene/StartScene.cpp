//
// Created by Hsuan on 2024/4/10.
//

#include "StartScene.hpp"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.hpp"
#include <allegro5/allegro_primitives.h>

#include "PasswordScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

int OnStage = 0;
int core_memories = 0;
int endless_score = 0;
bool skin1 = false;
bool skin2 = true;
// TODO HACKATHON-2 (1/3): You can imitate the 2 files: 'StartScene.hpp', 'StartScene.cpp' to implement your SettingsScene.
void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    fadeticks = 0; fadealpha = 0;
    timer = 0;
    house = al_load_bitmap("../Resource/images/house.png");
    AddNewObject(new Engine::Image("bgs/opening1.png", 0, 0, w, h, 0, 0));

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",  250, halfH / 2 + 200, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, 450, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 250, halfH + 120, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 48, 450, halfH + 170, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 250, halfH + 250, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::CreditsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Credits", "pirulen.ttf", 48, 450, halfH + 300, 0, 0, 0, 255, 0.5, 0.5));

    cricketInstance = AudioHelper::PlaySample("cricket.wav", true, AudioHelper::BGMVolume);
    sneakInstance = AudioHelper::PlaySample("sneak.wav", true, AudioHelper::BGMVolume);
}
void StartScene::Draw() const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::Draw();
    ALLEGRO_FONT* titlefont = Engine::Resources::GetInstance().GetFont("pirulen.ttf", 120).get();
    al_draw_filled_rectangle(70, 90, 1530, 230, al_map_rgba(0,0,0,255/1.75));
    al_draw_text(titlefont, al_map_rgba(84, 107, 171, 255 - fadealpha), 87.5, 86, 0, "Tower Defense");

    al_draw_tinted_scaled_bitmap(
        house,
        al_map_rgba(255, 255, 255, 255 - fadealpha),
        curX * 142, 94,    // Source X, Y (frame from sprite sheet)
        141, 93,        // Source width & height (one frame)
        halfW + 145, halfH + 15,                 // Destination X, Y
        580, 352,        // Destination width & height (scaled)
        0                                 // Flags
    );
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0,0,0, fadealpha));
}
void StartScene::Terminate() {
    AudioHelper::StopSample(cricketInstance);
    AudioHelper::StopSample(sneakInstance);
    cricketInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    sneakInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();

    IScene::Terminate();
}
void StartScene::Update(float deltaTime) {
    timer++;
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    if (timer % 15 == 0) {
        curX = (curX + 1) % 2;
    }
    if (fadeticks != 0) {
        fadeticks++;
        if (fadealpha <= 255) {
            fadealpha += 15;
            if (fadealpha >= 255) {
                fadealpha = 255;
            }
        }
        if (fadeticks == 40) {
            Engine::GameEngine::GetInstance().ChangeScene("stage-select");
        }
    }
}
void StartScene::PlayOnClick(int stage) {
    fadeticks = 1;
}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void StartScene::CreditsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("credits");
}