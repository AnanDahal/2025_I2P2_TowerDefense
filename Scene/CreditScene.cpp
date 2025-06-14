#include "CreditScene.h"

#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void CreditScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Label("CREDITS", "pirulen.ttf", 64, halfW, 50, 255, 255, 255, 255, 0.5, 0.5));

    float startY = 130;
    float lineHeight = 32; // space between lines
    float nameOffset = 20;
    float startY2 = 130;

    AddNewObject(new Engine::Label("Music from:", "pirulen.ttf", 48, halfW/2, startY, 255, 255, 255, 255, 0.5, 0));
    startY += nameOffset;
    AddNewObject(new Engine::Label("Violet Evergarden OST", "pirulen.ttf", 36, halfW / 2, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));
    AddNewObject(new Engine::Label("Sousou no Frieren OST", "pirulen.ttf", 36, halfW / 2, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));
    AddNewObject(new Engine::Label("Oregairu OST", "pirulen.ttf", 36, halfW/2, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));
    AddNewObject(new Engine::Label("Seishun Buta Yarou OST", "pirulen.ttf", 36, halfW/2, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&CreditScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("confession.wav", true, AudioHelper::BGMVolume);
}

void CreditScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void CreditScene::Draw() const {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    IScene::Draw();
}

void CreditScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
