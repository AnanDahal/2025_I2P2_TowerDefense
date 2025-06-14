#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PowersScene.h"
#include "Scene/SettingsScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

int whichPower = 0;

void PowersScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Label("POWERS", "pirulen.ttf", 48, halfW, 50, 255, 255, 255, 255, 0.5, 0.5));

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&PowersScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));
    //bomb power
    btn = new Engine::ImageButton("play/explosion-2.png", "play/explosion-3.png", 0, 120, 400, 400);
    btn->SetOnClickCallback(std::bind(&PowersScene::ChooseOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Atom Bomb", "pirulen.ttf", 36, 200, halfH + 125, 255, 255, 255, 255, 0.5, 0.5));
    //money power
    btn = new Engine::ImageButton("play/sell-icon-2.png", "play/sell-icon.png", 400, 120, 400, 400);
    btn->SetOnClickCallback(std::bind(&PowersScene::ChooseOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Raining Money", "pirulen.ttf", 36, 200 + 400, halfH + 125, 255, 255, 255, 255, 0.5, 0.5));
    //Time power
    btn = new Engine::ImageButton("play/turret-fire.png", "play/turret-fire.png", 800, 120, 400, 400);
    btn->SetOnClickCallback(std::bind(&PowersScene::ChooseOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Time Stop", "pirulen.ttf", 36, 200 + 800, halfH + 125, 255, 255, 255, 255, 0.5, 0.5));
    //AI
    btn = new Engine::ImageButton("play/turret-fire.png", "play/turret-fire.png", 1200, 120, 400, 400);
    btn->SetOnClickCallback(std::bind(&PowersScene::ChooseOnClick, this, 4));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("AI Help", "pirulen.ttf", 36, 200 + 1200, halfH + 125, 255, 255, 255, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample("sass.wav", true, AudioHelper::BGMVolume);
}
void PowersScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void PowersScene::Draw() const {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    IScene::Draw();
}

void PowersScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void PowersScene::ChooseOnClick(int power) {
    whichPower = power;
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}