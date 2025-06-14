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
#include "PasswordScene.h"
#include <ctime>
#include <iostream>
#include <allegro5/allegro_primitives.h>


using accountEntry = std::tuple<std::string, std::string, int, int, int>; // name, stage, memories, endless score
std::vector<accountEntry> accountEntries;

void PasswordScene::Initialize() {
    ticks = 0;
    playerName = LogInScene::getPlayerName();
    playerPassword = "";
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&PasswordScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));
    bgmInstance = AudioHelper::PlaySample("sass.wav", true, AudioHelper::BGMVolume);

    // Name input section
    AddNewObject(new Engine::Label("Enter your password:", "pirulen.ttf", 24, halfW, halfH / 2, 255, 255, 255, 255, 0.5, 0.5));

    // Create the name label (will show typed text)
    nameLabel = new Engine::Label("", "pirulen.ttf", 36, halfW, halfH / 2 + 65, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(nameLabel);

    // Submit button
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&PasswordScene::SubmitName, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Submit Password", "pirulen.ttf", 36, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
}

void PasswordScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void PasswordScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("log-in");
}

void PasswordScene::SubmitName() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    if (playerPassword.empty()) return;

    // Trim whitespace from password
    playerPassword.erase(playerPassword.begin(), std::find_if(playerPassword.begin(), playerPassword.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    playerPassword.erase(std::find_if(playerPassword.rbegin(), playerPassword.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), playerPassword.end());

    if (playerPassword.empty()) return;

    std::ifstream fin("../Resource/accounts.txt");

    std::string name;
    std::string password;
    int stage;
    int memories;
    int endless;
    bool skinone;
    bool skintwo;
    bool skinthree;
    bool skinfour;
    bool found = false;

    while (fin >> name >> password >> stage >> memories >> endless >> skinone >> skintwo >> skinthree >> skinfour) {
        if (name == playerName) {
            found = true;
            if (password == playerPassword) {
                OnStage = stage;
                core_memories = memories;
                endless_score = endless;
                skin1 = skinone;
                skin2 = skintwo;
                Engine::GameEngine::GetInstance().ChangeScene("stage-select");
                return;
            } else {
                AddNewObject(new Engine::Label("Wrong Password", "pirulen.ttf", 24, halfW, halfH / 2 + 300, 255, 255, 255, 255, 0.5, 0.5));
                return;
            }
        }
    }

    // If not found, create a new account
    if (!found) {
        std::ofstream fout("../Resource/accounts.txt", std::ios::app);
        if (fout) {
            fout << playerName << " " << playerPassword << " 0 0 0 0 0 0 0\n";
        }
        OnStage = 0;
        core_memories = 0;
        endless_score = 0;
        skin1 = false;
        skin2 = false;
        Engine::GameEngine::GetInstance().ChangeScene("stage-select"); //FARREL UPDATE THIS WHEN FINISH CUTSCENE
    }
}


void PasswordScene::Draw() const {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    IScene::Draw(); // Draw all existing objects first

    // Draw text box rectangle
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    al_draw_rectangle(halfW - 250, halfH / 2 +40, halfW + 250, halfH / 2 + 90,
                     al_map_rgb(255, 255, 255), 2);
    nameLabel->Text = playerPassword;
}

void PasswordScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    int len = strlen(al_keycode_to_name(keyCode));
    if (keyCode == ALLEGRO_KEY_BACKSPACE && !playerPassword.empty()) {
        playerPassword.pop_back();
        //nameLabel->Text = playerName;
    }
    else if (keyCode == ALLEGRO_KEY_ENTER) {
        SubmitName();
    }
    if (playerPassword.length() >= 12) {
        return;
    }
    if (len == 1) {
        playerPassword += al_keycode_to_name(keyCode);
    }
}