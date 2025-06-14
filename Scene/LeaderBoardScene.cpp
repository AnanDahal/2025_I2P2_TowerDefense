#include "Scene/LeaderBoardScene.h"
#include <allegro5/allegro_audio.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include <iostream>

void LeaderBoardScene::LoadScores() {
    scores.clear();
    std::ifstream file("../Resource/scoreboard.txt");
    if (!file.is_open()) {
        std::cerr << "[LeaderBoardScene] Failed to open “scoreboard.txt” in working dir\n";
        return;
    }

    std::string name, date, time;
    int score;
    while (file >> name >> score >> date >> time) {
        scores.emplace_back(name, score, date + " " + time);
    }

    // debug‐dump what we actually read:
    for (auto const& [n, s, dt] : scores) {
        std::cout << "[LB] Loaded “" << n << "” → " << s
                  << " (" << dt << ")\n";
    }

    std::sort(scores.begin(), scores.end(),
        [](auto const& a, auto const& b) {
            return std::get<1>(a) > std::get<1>(b);
        });
}


void LeaderBoardScene::ClearScoreObjects() {
    // 1) Remove all labels except the BACK label
    for (auto it = objects.begin(); it != objects.end();) {
        if (auto* lbl = dynamic_cast<Engine::Label*>(it->second)) {
            if (lbl != backLabel) {
                auto cur = it++;
                RemoveObject(cur);
                continue;
            }
        }
        ++it;
    }

    // 2) Remove all ImageButton controls except BACK
    for (auto ctrlIt = controls.begin(); ctrlIt != controls.end();) {
        auto* btn = dynamic_cast<Engine::ImageButton*>(ctrlIt->second);
        if (btn && btn != backControl) {
            // find the same button in the objects list by down‐casting each object to IControl*
            auto objIt = std::find_if(
                objects.begin(), objects.end(),
                [&](auto const& p){
                    auto* objAsCtrl = dynamic_cast<Engine::IControl*>(p.second);
                    return objAsCtrl == btn;
                }
            );

            // advance ctrlIt before erasing
            auto curCtrl = ctrlIt++;
            if (objIt != objects.end()) {
                RemoveControlObject(curCtrl, objIt);
            } else {
                // fallback: orphaned control, just erase control entry
                controls.erase(curCtrl);
            }
            continue;
        }
        ++ctrlIt;
    }
}




void LeaderBoardScene::DisplayCurrentPage() {
    ClearScoreObjects();

    al_clear_to_color(al_map_rgb(0, 0, 0));
    auto& eng = Engine::GameEngine::GetInstance();
    int w = eng.GetScreenSize().x, h = eng.GetScreenSize().y;
    int halfW = w/2;

    // Title
    AddNewObject(new Engine::Label(
        "SCOREBOARD", "pirulen.ttf", 63, halfW, 50,
        255,255,255,255, 0.5,0.5));

    int start = currentPage * ENTRIES_PER_PAGE;
    int max  = static_cast<int>(scores.size());
    int end   = std::min(start + ENTRIES_PER_PAGE, max);

    for (int i = start; i < end; i++) {
        std::stringstream ss;
        ss << (i + 1) << ". "
           << std::get<0>(scores[i])
           << " - " << std::get<1>(scores[i])
           << " (" << std::get<2>(scores[i]) << ")";
        AddNewObject(new Engine::Label(
            ss.str(), "pirulen.ttf", 48,
            halfW, 120 + (i - start)*50,
            255,255,255,255, 0.5,0.5));
    }

    int maxPage = (max - 1) / ENTRIES_PER_PAGE;
    int btnY = h - 100;

    // Prev button
    if (currentPage > 0) {
        Engine::ImageButton* prev = new Engine::ImageButton(
            "stage-select/dirt.png", "stage-select/floor.png",
             50, btnY, 100, 50);
        prev->SetOnClickCallback(
            std::bind(&LeaderBoardScene::PrevPageOnClick, this));
        AddNewControlObject(prev);
        AddNewObject(new Engine::Label("<", "pirulen.ttf", 32,
            100, btnY+25, 0,255,0,255, 0.5,0.5));
    }

    // Next button
    if (currentPage < maxPage) {
        Engine::ImageButton* next = new Engine::ImageButton(
            "stage-select/dirt.png", "stage-select/floor.png",
             w - 150, btnY, 100, 50);
        next->SetOnClickCallback(
            std::bind(&LeaderBoardScene::NextPageOnClick, this));
        AddNewControlObject(next);
        AddNewObject(new Engine::Label(">", "pirulen.ttf", 32,
            w - 100, btnY+25, 0,255,0,255, 0.5,0.5));
    }
}

void LeaderBoardScene::Initialize() {
    bgmInstance = AudioHelper::PlaySample("crown.wav", true, AudioHelper::BGMVolume);
    // Back button
    auto& eng = Engine::GameEngine::GetInstance();
    int w = eng.GetScreenSize().x, h = eng.GetScreenSize().y;
    int halfW = w/2;

    Engine::ImageButton* back = new Engine::ImageButton(
        "stage-select/dirt.png","stage-select/floor.png",
         halfW-200, h-150, 400,100);
    back->SetOnClickCallback(
        std::bind(&LeaderBoardScene::BackOnClick, this, 0));
    AddNewControlObject(back);
    AddNewObject(new Engine::Label(
        "BACK","pirulen.ttf",48,halfW,h-100,
         0,0,0,255,0.5,0.5));

    backControl = back;
    backLabel = new Engine::Label(
    "BACK","pirulen.ttf",48,halfW,h-100,
    0,0,0,255,0.5,0.5);
    AddNewObject(backLabel);


    LoadScores();
    currentPage = 0;
    DisplayCurrentPage();
}

void LeaderBoardScene::BackOnClick(int) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void LeaderBoardScene::NextPageOnClick() {
    int maxPage = (int(scores.size()) - 1) / ENTRIES_PER_PAGE;
    if (currentPage < maxPage) {
        currentPage++;
        DisplayCurrentPage();
    }
}

void LeaderBoardScene::PrevPageOnClick() {
    if (currentPage > 0) {
        currentPage--;
        DisplayCurrentPage();
    }
}


void LeaderBoardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
