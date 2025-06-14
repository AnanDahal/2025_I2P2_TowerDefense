#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PowersScene.h"
#include "Scene/ShopScene.h"

#include <iostream>

#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

//MOVE TO STARTSCENE


void ShopScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Label("SHOP", "pirulen.ttf", 48, halfW, 50, 255, 255, 255, 255, 0.5, 0.5));

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&ShopScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));
    //bomb power
    btn = new Engine::ImageButton("play/Turret upgrades/turret-1-u1.png", "play/Turret upgrades/turret-1-u1.png", 200, 120, 400, 400);
    btn->SetOnClickCallback(std::bind(&ShopScene::ChooseOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Turret Skin Pack", "pirulen.ttf", 36, 400, halfH + 125, 255, 255, 255, 255, 0.5, 0.5));
    //money power
    btn = new Engine::ImageButton("play/Turret upgrades/turret-1-u2.png", "play/Turret upgrades/turret-1-u2.png", 1000, 120, 400, 400);
    btn->SetOnClickCallback(std::bind(&ShopScene::ChooseOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Deluxe Turret Skin Pack", "pirulen.ttf", 36, 800 + 400, halfH + 125, 255, 255, 255, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label(
        ("Core Memory: " + std::to_string(core_memories)).c_str(),
        "pirulen.ttf", 30, 10, 10, 255, 255, 255, 255, 0.0, 0.0
        ));  


    bgmInstance = AudioHelper::PlaySample("whistle.wav", true, AudioHelper::BGMVolume-0.75);
    bellInstance = AudioHelper::PlaySample("bell.wav", false, AudioHelper::BGMVolume);
}
void ShopScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    bellInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void ShopScene::Draw() const {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    IScene::Draw();
}

void ShopScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void ShopScene::ChooseOnClick(int skin) {
    if (skin == 1 && core_memories >= 100) {
        skin1 = true;
        core_memories -= 100;
    }
    else if (skin == 2 && core_memories >= 500) {
        skin2 = true;
        core_memories -= 500;
    }
}