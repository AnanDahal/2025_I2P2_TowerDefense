#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>


#include <allegro5/allegro_primitives.h>

#include "LogInScene.h"

void WinScene::Initialize() {
    ticks = 0;
    playerName = "";
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
    // AddNewObject(new Engine::Label("Enter Name: ", "pirulen.ttf", 32, halfW - 350, halfH / 4 + 65, 255, 255, 255, 255, 0.5, 0.5));
    //
    // nameLabel = new Engine::Label(playerName, "pirulen.ttf", 32, halfW, halfH / 4 + 65, 255, 255, 255, 255, 0.5, 0.5);
    // AddNewObject(nameLabel);


    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("final.wav");
    //SAVE PLAYER DATA!!!!!
    std::ifstream fin("../Resource/accounts.txt");
    std::vector<std::string> lines;
    std::string name, password;
    int stage, memories, endless;
    bool skinone, skintwo, skinthree, skinfour;
    playerName = LogInScene::getPlayerName();

    // Read and update
    while (fin >> name >> password >> stage >> memories >> endless >> skinone >> skintwo >> skinthree >> skinfour) {
        if (name == playerName) {
            // Update current player's data
            stage = OnStage;
            memories = core_memories;
            endless = endless_score;
            skinone = skin1;
            skintwo = skin2;
        }

        std::ostringstream oss;
        oss << name << " " << password << " "
            << stage << " " << memories << " " << endless << " "
            << skinone << " " << skintwo << " " << skinthree << " " << skinfour;
        lines.push_back(oss.str());
    }
    fin.close();

    // Write back all lines
    std::ofstream fout("../Resource/accounts.txt");
    for (const auto& line : lines) {
        fout << line << "\n";
    }

    std::ofstream file("../Resource/scoreboard.txt", std::ios::app);
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    std::string time = ss.str();
    if (file.is_open()) {
        file << playerName << " " << endless_score << " " << time << std::endl;
        file.close();
    }
}
void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}

void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
        }
}

void WinScene::OnKeyDown(int keyCode) {
    // if (keyCode == ALLEGRO_KEY_BACKSPACE) {
    //     if (!playerName.empty()) {
    //         playerName.pop_back();
    //         nameLabel->Text = playerName;
    //     }
    // }
    // else if (keyCode == ALLEGRO_KEY_ENTER) {
    //     if (!playerName.empty()) {
    //         // Get the PlayScene instance and score
    //         PlayScene* playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    //         if (playScene) {
    //             int score = playScene->GetScore();
    //
    //             // Open file in append mode with explicit path
    //             std::ofstream file("../Resource/scoreboard.txt", std::ios::app);
    //             auto now = std::chrono::system_clock::now();
    //             auto in_time_t = std::chrono::system_clock::to_time_t(now);
    //             std::stringstream ss;
    //             ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    //             std::string time = ss.str();
    //             if (file.is_open()) {
    //                 file << playerName << " " << score << " " << time << std::endl;
    //                 file.close();
    //             }
    //         }
    //
    //         // Change scene after saving
    //         Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    //     }
    // }
    // else if ((keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) || keyCode == ALLEGRO_KEY_SPACE) {
    //     if (playerName.length() < 15) {  // Limit name length to 15 characters
    //         if (keyCode == ALLEGRO_KEY_SPACE) {
    //             playerName += "_";
    //         } else {
    //             char c = static_cast<char>('A' + (keyCode - ALLEGRO_KEY_A));
    //             playerName += c;
    //         }
    //         nameLabel->Text = playerName;
    //     }
    // }
}


void WinScene::Draw() const {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    IScene::Draw();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;



    // Draw the rounded rectangle
    // al_draw_rounded_rectangle(halfW - 200,      // x1
    //                         halfH / 4 + 30,         // y1
    //                         halfW + 200,        // x2
    //                         halfH / 4 + 100,         // y2
    //                         15, 15,             // rx, ry
    //                         al_map_rgb(255, 255, 255),  // color
    //                         3);                 // thickness
}



void WinScene::BackOnClick(int stage) {
    // Change to select scene.
    PlayScene* playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    if (playScene && playScene->MapId == 6) {
        Engine::GameEngine::GetInstance().ChangeScene("credits");
    } else {
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
}