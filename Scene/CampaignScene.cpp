//
// Created by Farrel on 6/12/2025.
//

#include "CampaignScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "AfterScene.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "BeforeScene.h"
#include "StageSelectScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void CampaignScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 350, halfH / 2 - 150, 400, 100);
    btn->SetOnClickCallback(std::bind(&CampaignScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 1", "pirulen.ttf", 48, halfW - 250, halfH / 2 - 100, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 350, halfH / 2, 400, 100);
    btn->SetOnClickCallback(std::bind(&CampaignScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 2", "pirulen.ttf", 48, halfW - 250, halfH / 2 + 50, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 350, halfH / 2 + 150, 400, 100);
    btn->SetOnClickCallback(std::bind(&CampaignScene::PlayOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 3", "pirulen.ttf", 48, halfW - 250, halfH / 2 + 200, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 50, halfH / 2 - 150, 400, 100);
    btn->SetOnClickCallback(std::bind(&CampaignScene::PlayOnClick, this, 4));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 4", "pirulen.ttf", 48, halfW + 250, halfH / 2 - 100, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 50, halfH / 2, 400, 100);
    btn->SetOnClickCallback(std::bind(&CampaignScene::PlayOnClick, this, 5));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 5", "pirulen.ttf", 48, halfW + 250, halfH / 2 + 50, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 50, halfH / 2 + 150, 400, 100);
    btn->SetOnClickCallback(std::bind(&CampaignScene::PlayOnClick, this, 6));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 6", "pirulen.ttf", 48, halfW + 250, halfH / 2 + 200, 0, 0, 0, 255, 0.5, 0.5));

    //AddNewObject(new Engine::Image("icon.png", halfW - 200, halfH * 3 / 2, 0, 0, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&CampaignScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("mono.wav", true, AudioHelper::BGMVolume);
}
void CampaignScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void CampaignScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void CampaignScene::PlayOnClick(int stage) {
    // Check if this is a story stage (1-5)
    if (stage >= 1 && stage <= 5) {
        // For stages with intro story, go to BeforeScene
        if (OnStage < stage) {
            OnStage = stage;
        }
        BeforeScene *beforescene = dynamic_cast<BeforeScene *>(Engine::GameEngine::GetInstance().GetScene("before"));
        if (beforescene) {
            beforescene->storyid = stage;
            Engine::GameEngine::GetInstance().ChangeScene("before");
        }
    } 
    // Special case for stage 6 (no before scene)
    else if (stage == 6) {
        // Go directly to play scene for stage 6
        PlayScene *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
        if (scene) {
            scene->MapId = stage;
            Engine::GameEngine::GetInstance().ChangeScene("play");
        }
    }
    // For any other stages
    else {
        PlayScene *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
        if (scene) {
            scene->MapId = stage;
            Engine::GameEngine::GetInstance().ChangeScene("play");
        }
    }
}