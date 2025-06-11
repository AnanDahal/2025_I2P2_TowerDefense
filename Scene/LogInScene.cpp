#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/LogInScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include <fstream>
#include <algorithm>
#include "WinScene.hpp"
#include "Scene/PasswordScene.h"
#include <ctime>
#include <allegro5/allegro_primitives.h>


// using accountEntry = std::tuple<std::string, int, int, int>; // name, stage, memories, endless score
// std::vector<accountEntry> accountEntries;

void LogInScene::Initialize() {
    ticks = 0;
    playerName = "";
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&LogInScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));
    bgmInstance = AudioHelper::PlaySample("sass.wav", true, AudioHelper::BGMVolume);

    // Name input section
    AddNewObject(new Engine::Label("Enter your name:", "pirulen.ttf", 24, halfW, halfH / 2, 255, 255, 255, 255, 0.5, 0.5));

    // Create the name label (will show typed text)
    nameLabel = new Engine::Label("", "pirulen.ttf", 36, halfW, halfH / 2 + 65, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(nameLabel);

    // Submit button
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&LogInScene::SubmitName, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Submit User", "pirulen.ttf", 36, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
}

void LogInScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void LogInScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void LogInScene::SubmitName() {
    if (playerName.empty()) return;

    // Trim whitespace from name
    playerName.erase(playerName.begin(), std::find_if(playerName.begin(), playerName.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    playerName.erase(std::find_if(playerName.rbegin(), playerName.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), playerName.end());

    if (playerName.empty()) return;
    //PasswordScene::SetPlayerName(playerName);
    // Go to stage select
    Engine::GameEngine::GetInstance().ChangeScene("password");
}

void LogInScene::Draw() const {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    IScene::Draw(); // Draw all existing objects first

    // Draw text box rectangle
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    al_draw_rectangle(halfW - 250, halfH / 2 +40, halfW + 250, halfH / 2 + 90,
                     al_map_rgb(255, 255, 255), 2);
    nameLabel->Text = playerName;
}

void LogInScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    int len = strlen(al_keycode_to_name(keyCode));
    if (keyCode == ALLEGRO_KEY_BACKSPACE && !playerName.empty()) {
        playerName.pop_back();
        //nameLabel->Text = playerName;
    }
    else if (keyCode == ALLEGRO_KEY_ENTER) {
        SubmitName();
    }
    if (playerName.length() >= 12) {
        return;
    }
    if (len == 1) {
        playerName += al_keycode_to_name(keyCode);
    }
}

std::string LogInScene::getPlayerName() {
    return playerName;
}