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

    AddNewObject(new Engine::Label("Music from:", "pirulen.ttf", 48, halfW/2 - 150, startY, 255, 255, 255, 255, 0.5, 0));
    startY += nameOffset;
    AddNewObject(new Engine::Label("Violet Evergarden OST", "pirulen.ttf", 36, halfW / 2 - 150, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));
    AddNewObject(new Engine::Label("Sousou no Frieren OST", "pirulen.ttf", 36, halfW / 2 - 150, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));
    AddNewObject(new Engine::Label("Oregairu OST", "pirulen.ttf", 36, halfW/2 - 150, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));
    AddNewObject(new Engine::Label("Seishun Buta Yarou OST", "pirulen.ttf", 36, halfW/2 - 150, startY += lineHeight, 200, 200, 200, 255, 0.5, 0));

        // Column positions
    float col1X = halfW / 2;
    float col2X = halfW + 50;
    float col3X = halfW + (w / 3) + 20;

    // Reset startY for contributor sections
    float startY_Farrel = 130;
    float startY_Ming = 130;
    float startY_Anan = 130;
    nameOffset = 20;
    lineHeight = 28;

    // Farrel
    AddNewObject(new Engine::Label("Farrel:", "pirulen.ttf", 36, col2X - 150, startY_Farrel, 255, 255, 255, 255, 0.5, 0));
    startY_Farrel += nameOffset;
    std::vector<std::string> farrelCredits = {
        "- UI Drawing (all assets besides scene characters)",
        "- Random Tower Placement",
        "- All Cutscenes",
        "- Cutscene System",
        "- Dialogue System",
        "- Enemy category System",
        "- AOE Turret",
        "- Army enemy",
        "- Animation",
        "- Void Tile",
    };
    for (const auto& line : farrelCredits) {
        AddNewObject(new Engine::Label(line, "pirulen.ttf", 28, col2X - 150, startY_Farrel += lineHeight, 200, 200, 200, 255, 0.5, 0));
    }

    // Ming-Hao
    AddNewObject(new Engine::Label("Ming-Hao:", "pirulen.ttf", 36, col3X - 150, startY_Ming, 255, 255, 255, 255, 0.5, 0));
    startY_Ming += nameOffset;
    std::vector<std::string> mingCredits = {
        "- UI Drawing (scene characters)",
        "- Bomb power, Money power, and cooldown system",
        "- Tower lock System",
        "- Account log in, register, saving, and editing",
        "- Boss, MiniBoss, Miss, BiggerCarrier, Carrier Enemies",
        "- Buff, Farm, Slow, Sniper, Tank Killer, Boss Killer Turrets",
        "- Upgrade System",
    };
    for (const auto& line : mingCredits) {
        AddNewObject(new Engine::Label(line, "pirulen.ttf", 28, col3X - 150, startY_Ming += lineHeight, 200, 200, 200, 255, 0.5, 0));
    }

    // Anan
    startY_Anan = startY_Ming + lineHeight * 2; // Add spacing below Ming-Hao's section
    AddNewObject(new Engine::Label("Anan:", "pirulen.ttf", 36, col3X - 150, startY_Anan, 255, 255, 255, 255, 0.5, 0));
    startY_Anan += nameOffset;
    std::vector<std::string> ananCredits = {
        "- AI map generation",
        "- AI auto placing power",
        "- AI wave generation",
        "- Time stopping power",
        "- Healing Turret",
        "- Invisible enemy, Tank enemy",
        "- Void Tile",
        "- Skin System",
    };
    for (const auto& line : ananCredits) {
        AddNewObject(new Engine::Label(line, "pirulen.ttf", 28, col3X - 150, startY_Anan += lineHeight, 200, 200, 200, 255, 0.5, 0));
    }

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
