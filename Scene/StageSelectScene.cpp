#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"

#include <iostream>
#include <allegro5/allegro_primitives.h>

#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void StageSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    bg = al_load_bitmap("../Resource/images/bgs/stage.png");
    db = al_load_bitmap("../Resource/images/Dbubble.png");
    bgX = dbX = 0;
    ticks = timers = 0;
    fadealpha = 255;
    changeto = stagenum = 0;
    fadeout = false;
    Engine::ImageButton *btn;

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 200, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scores", "pirulen.ttf", 48, halfW, halfH / 2 - 150, 0, 0, 0, 255, 0.5, 0.5));


    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Story Mode", "pirulen.ttf", 48, halfW, halfH / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Death Match", "pirulen.ttf", 48, halfW, halfH / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 350, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::BackOnClick, this, 4));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH / 2 + 400, 0, 0, 0, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample("snore.wav", true, AudioHelper::BGMVolume);
}
void StageSelectScene::Draw() const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    //background
    al_draw_scaled_bitmap(bg, bgX*630, 0, 630, 328, 0, 0, w, h, 0);
    //dream bubble or db
    al_draw_scaled_bitmap(db, dbX*630, 0, 630, 328, 0, 0, w, h, 0);
    IScene::Draw();
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0,0,0,fadealpha));
}
void StageSelectScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void StageSelectScene::Update(float deltaTime) {
    timers++;
    IScene::Update(deltaTime);

    if (timers % 15 == 0) {
        bgX = (bgX + 1) % 2;
        dbX = (dbX + 1) % 3;
    }
    //fade in n out
    if (!fadeout && fadealpha > 0) {
        fadealpha -= 15;
        if (fadealpha < 0) {
            fadealpha = 0;
        }
    }
    else if (fadeout && fadealpha < 255) {
        fadealpha += 15;
        //scene changes
        if (fadealpha >= 255) {
            if (changeto == 1) {
                Engine::GameEngine::GetInstance().ChangeScene("opening");
            }
            else if (changeto == 2) {
                PlayScene *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
                scene->MapId = stagenum;
                // change the name from win to play for debugging (this is for github action)
                Engine::GameEngine::GetInstance().ChangeScene("play");
            }
            else if (changeto == 3) {
                Engine::GameEngine::GetInstance().ChangeScene("scoreboard-scene");
            }
        }
    }
}
void StageSelectScene::BackOnClick(int stage) {
    changeto = 1;
    fadeout = true;
}
void StageSelectScene::PlayOnClick(int stage) {
    changeto = 2; stagenum = stage;
    fadeout = true;
}
void StageSelectScene::ScoreboardOnClick(int stage) {
    changeto = 3;
    fadeout = true;
}
void StageSelectScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void StageSelectScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}