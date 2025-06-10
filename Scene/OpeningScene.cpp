//
// Created by Farrel on 6/10/2025.
//

#include "OpeningScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <allegro5/allegro_primitives.h>
#include <bits/ostream.tcc>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void OpeningScene::Initialize() {
    fadeticks = 75;
    timer = 0; tick = 0;
    curX = 0; curY = 0;
    charframe = 0;
    charx = -1.0;
    fadealpha = 255; alpha = 0;
    done = false; show = false; played = false;

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    house = al_load_bitmap("../Resource/images/house.png");
    chara = al_load_bitmap("../Resource/images/character_walk.png");

    AddNewObject(new Engine::Image("bgs/opening1.png", 0, 0, w, h, 0, 0));
    bgmInstance = AudioHelper::PlaySample("cricket.wav", true, AudioHelper::BGMVolume);
}
void OpeningScene::Draw() const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    IScene::Draw();

    ALLEGRO_FONT* titlefont = Engine::Resources::GetInstance().GetFont("pirulen.ttf", 120).get();
    ALLEGRO_FONT* smallerfont = Engine::Resources::GetInstance().GetFont("pirulen.ttf", 40).get();
    //remove fadealphas
    al_draw_filled_rectangle(70, 90, 1530, 230, al_map_rgba(0,0,0,alpha/1.75));
    al_draw_text(titlefont, al_map_rgba(84, 107, 171, alpha), 87.5, 86, 0, "Tower Defense");
    if (show) {
        al_draw_text(smallerfont, al_map_rgba(255, 255, 255, alpha), 200, halfH + 86, 0, "Press [ENTER]");
        al_draw_text(smallerfont, al_map_rgba(255, 255, 255, alpha), 275, halfH + 150, 0, "to play");
    }
    //house
    al_draw_tinted_scaled_bitmap(
        house,
        al_map_rgba(255, 255, 255, 255 - fadealpha),
        curX * 142, curY,    // Source X, Y (frame from sprite sheet)
        141, 93,        // Source width & height (one frame)
        halfW + 145, halfH + 15,                 // Destination X, Y
        580, 352,        // Destination width & height (scaled)
        0                                 // Flags
    );
    //character
    al_draw_tinted_scaled_bitmap(
        chara,
        al_map_rgba(255, 255, 255, 255 - alpha),
        charframe*32, 32,    // Source X, Y (frame from sprite sheet)
        32, 32,        // Source width & height (one frame)
        charx, 660,                 // Destination X, Y
        96, 96,        // Destination width & height (scaled)
        1                                 // Flags
    );
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0,0,0, fadealpha));
}
void OpeningScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void OpeningScene::Update(float deltaTime) {
    timer++;
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    if (timer % 15 == 0) {
        curX = (curX + 1) % 2;
    }
    if (timer % 10 == 0) {
        charframe = (charframe+1) % 4;
        if (charx == 0) {
            charframe = 0;
        }
    }

    if (tick < 200) {
        tick++;
        charx += 5;
    }
    //fade
    if (fadeticks >= 0) {
        fadeticks--;
        if (fadealpha > 0) {
            fadealpha -= 5;
            if (fadealpha < 0) {
                fadealpha = 0;
            }
        }
    }
    //fade done

    if (tick == 200) {
        curY = 94;
        if (timer % 30 == 0) show = !show;
        if (!played) {
            AudioHelper::PlayAudio("closedoor.wav");
            played = true;
        }
        if (alpha <= 255) {
            alpha += 5;
            if (alpha >= 255) {
                alpha = 255;
                done = true;
            }
        }
    }
    IScene::Update(deltaTime);
}
void OpeningScene::OnKeyDown(int keycode) {
    IScene::OnKeyDown(keycode);
    if (keycode == ALLEGRO_KEY_ENTER) {
        if (!done) {
            tick = 200;
            alpha = 255;
            fadealpha = 0;
            curY = 94;
            done = true;
        }
        else {
            Engine::GameEngine::GetInstance().ChangeScene("start");
        }
    }
}