//
// Created by Farrel on 6/14/2025.
//

#include "ClosingScene.h"
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
#include "WinScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void ClosingScene::Initialize() {
    fadeticks = 75;
    timer = 0;
    fadealpha = 255;
    
    // Add variables for the finalscene bitmap animation
    finalSceneX = 0;
    finalSceneY = 0;
    
    // Initialize weird scene variables
    showWeirdScene = false;
    weirdSceneTimer = 0;
    weirdSceneAlpha = 0;
    weirdSceneDissolveIn = true;
    
    // Initialize panda fade and weird screen variables
    showPandaFade = false;
    showWeirdScreen = false;
    pandaFadeFrame = 0;
    pandaFadeAlpha = 0;
    weirdScreenAlpha = 0;
    pandaFadeTimer = 0;
    
    // Initialize final fade out
    finalFadeOut = false;
    finalFadeTimer = 0;
    finalFadeAlpha = 0;

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    
    // Load all required bitmaps
    finalScene = al_load_bitmap("../Resource/images/finalscene.png");
    weirdScene = al_load_bitmap("../Resource/images/weird_scene.png");
    weirdScreen = al_load_bitmap("../Resource/images/weird_screen.png");
    pandaFade = al_load_bitmap("../Resource/images/pandafade.png");

    // Keep only the cricket sound
    soundInstance = AudioHelper::PlaySample("sad.wav", true, AudioHelper::BGMVolume);
}
void ClosingScene::Draw() const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    IScene::Draw();
    
    // Always clear to white as the base background
    al_clear_to_color(al_map_rgb(255, 255, 255));
    
    // Draw the finalscene bitmap with animation if no other scenes are showing
    if (finalScene && !showWeirdScene && !showPandaFade && !showWeirdScreen && !finalFadeOut) {
        al_draw_scaled_bitmap(
            finalScene,
            finalSceneX * 980, finalSceneY * 710,  // Source X, Y (frame from sprite sheet)
            980, 710,                              // Source width & height (one frame)
            0, 0,                                  // Destination X, Y
            w, h,                                  // Destination width & height (scaled to screen)
            0                                      // Flags
        );
    }
    
    // Draw the weird scene if it's time to show it
    if (weirdScene && showWeirdScene) {
        al_draw_tinted_scaled_bitmap(
            weirdScene,
            al_map_rgba(255, 255, 255, weirdSceneAlpha),  // Apply alpha for dissolve effect
            0, 0,                                          // Source X, Y
            al_get_bitmap_width(weirdScene),               // Source width
            al_get_bitmap_height(weirdScene),              // Source height
            0, 0,                                          // Destination X, Y
            w, h,                                          // Destination width & height (scaled to screen)
            0                                              // Flags
        );
    }
    
    // Draw the weird screen with its own alpha value
    if (weirdScreen && showWeirdScreen) {
        al_draw_tinted_scaled_bitmap(
            weirdScreen,
            al_map_rgba(255, 255, 255, weirdScreenAlpha),  // Apply alpha for dissolve effect
            0, 0,                                           // Source X, Y
            al_get_bitmap_width(weirdScreen),               // Source width
            al_get_bitmap_height(weirdScreen),              // Source height
            0, 0,                                           // Destination X, Y
            w, h,                                           // Destination width & height (scaled to screen)
            0                                               // Flags
        );
    }
    
    // Draw the panda fade with its own alpha value
    if (pandaFade && showPandaFade) {
        al_draw_tinted_scaled_bitmap(
            pandaFade,
            al_map_rgba(255, 255, 255, pandaFadeAlpha),  // Apply alpha for dissolve effect
            pandaFadeFrame * 980, 0,                     // Source X, Y (frame from sprite sheet)
            980, 710,                                    // Source width & height (one frame)
            0, 0,                                        // Destination X, Y
            w, h,                                        // Destination width & height (scaled to screen)
            0                                            // Flags
        );
    }
    
    // Draw the initial fade effect
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, fadealpha));
    
    // Draw the final fade to black effect - this will gradually dissolve to black
    if (finalFadeOut) {
        al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, finalFadeAlpha));
    }
}
void ClosingScene::Terminate() {
    // Stop and clean up cricket sound
    AudioHelper::StopSample(soundInstance);
    soundInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    
    // Clean up bitmaps
    if (finalScene) {
        al_destroy_bitmap(finalScene);
        finalScene = nullptr;
    }
    if (weirdScene) {
        al_destroy_bitmap(weirdScene);
        weirdScene = nullptr;
    }
    if (weirdScreen) {
        al_destroy_bitmap(weirdScreen);
        weirdScreen = nullptr;
    }
    if (pandaFade) {
        al_destroy_bitmap(pandaFade);
        pandaFade = nullptr;
    }
    
    IScene::Terminate();
}
void ClosingScene::Update(float deltaTime) {
    timer++;
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Check if it's time to show the weird scene - only if nothing else is showing
    if (timer % 600 == 0 && !showWeirdScene && !showPandaFade && !showWeirdScreen && !finalFadeOut) {
        showWeirdScene = true;
        weirdSceneTimer = 0;
        weirdSceneAlpha = 0;  // Start with 0 alpha for dissolve-in
        weirdSceneDissolveIn = true;  // Start in dissolve-in phase
    }
    
    // Handle weird scene timing and effects
    if (showWeirdScene) {
        weirdSceneTimer++;
        
        // Handle dissolve-in effect for weird scene
        if (weirdSceneDissolveIn) {
            weirdSceneAlpha += 5;  // Gradually increase alpha for dissolve-in
            if (weirdSceneAlpha >= 255) {
                weirdSceneAlpha = 255;
                weirdSceneDissolveIn = false;  // Dissolve-in complete
            }
        }
        // After 4 seconds (assuming 60 FPS, so 240 frames), start fading out
        else if (weirdSceneTimer > 240) {
            weirdSceneAlpha -= 5;  // Decrease alpha to make it fade out
            
            // When fully transparent, transition to weird screen and panda fade
            if (weirdSceneAlpha <= 0) {
                weirdSceneAlpha = 0;
                showWeirdScene = false;
                
                // Start showing weird screen and panda fade with dissolve-in effect
                showWeirdScreen = true;
                showPandaFade = true;
                weirdScreenAlpha = 0;
                pandaFadeAlpha = 0;
                pandaFadeTimer = 0;
            }
        }
    }
    
    // Handle weird screen and panda fade timing and effects
    if (showWeirdScreen || showPandaFade) {
        pandaFadeTimer++;
        
        // Handle dissolve-in effect for both
        if (pandaFadeTimer <= 60) {  // First second for dissolve-in
            weirdScreenAlpha += 255 / 60;  // Gradually increase alpha
            pandaFadeAlpha += 255 / 60;    // Gradually increase alpha
            
            if (weirdScreenAlpha > 255) weirdScreenAlpha = 255;
            if (pandaFadeAlpha > 255) pandaFadeAlpha = 255;
        }
        // After 4 seconds (240 frames), start fading out weird screen
        else if (pandaFadeTimer > 240 && showWeirdScreen) {
            weirdScreenAlpha -= 5;  // Decrease alpha to make weird screen fade out
            
            if (weirdScreenAlpha <= 0) {
                weirdScreenAlpha = 0;
                showWeirdScreen = false;
                // Reset timer to track time after weird screen is gone
                pandaFadeTimer = 0;
            }
        }
        // After weird screen is gone, wait 4 seconds (240 frames) before fading out panda
        else if (pandaFadeTimer > 240 && !showWeirdScreen && showPandaFade) {
            pandaFadeAlpha -= 5;  // Decrease alpha to make panda fade out
            
            if (pandaFadeAlpha <= 0) {
                pandaFadeAlpha = 0;
                showPandaFade = false;
                
                // Start the final fade to black after 2 seconds
                finalFadeOut = true;
                finalFadeTimer = 0;
                finalFadeAlpha = 0;  // Start with transparent black
            }
        }
        
        // Animate panda fade frames throughout
        if (pandaFadeTimer % 30 == 0 && showPandaFade) {
            pandaFadeFrame = (pandaFadeFrame + 1) % 2;  // Toggle between 0 and 1
        }
    }
    
    // Handle final fade to black
    if (finalFadeOut) {
        finalFadeTimer++;
        
        // Wait 2 seconds (120 frames) before starting the fade
        if (finalFadeTimer > 120) {
            // Gradually increase alpha for dissolve to black (slower for smoother transition)
            finalFadeAlpha += 3;  
            
            if (finalFadeAlpha >= 255) {
                finalFadeAlpha = 255;
                
                // Wait another second after fully black before changing scene
                if (finalFadeTimer > 120 + 60) {
                    // Transition to WinScene
                    WinScene *winScene = dynamic_cast<WinScene *>(Engine::GameEngine::GetInstance().GetScene("win"));
                    if (winScene) {
                        winScene->winstage = changeid;
                    }
                    Engine::GameEngine::GetInstance().ChangeScene("win");
                }
            }
        }
    }

    // Only update finalscene animation if we're showing it and nothing else is active
    if (!showWeirdScene && !showPandaFade && !showWeirdScreen && !finalFadeOut) {
        // Update finalscene animation
        if (timer % 20 == 0) {
            finalSceneX = (finalSceneX + 1) % 2;  // 2 frames per x
        }
        
        if (timer % 200 == 0) {
            finalSceneY = (finalSceneY + 1) % 3;  // 3 lines for y
        }
    }
    
    // Keep the initial fade-in effect
    if (fadeticks >= 0) {
        fadeticks--;
        if (fadealpha > 0) {
            fadealpha -= 5;
            if (fadealpha < 0) {
                fadealpha = 0;
            }
        }
    }
    
    IScene::Update(deltaTime);
}
void ClosingScene::OnKeyDown(int keycode) {
    IScene::OnKeyDown(keycode);
    if (keycode == ALLEGRO_KEY_ENTER) {
        WinScene *winScene = dynamic_cast<WinScene *>(Engine::GameEngine::GetInstance().GetScene("win"));
        if (winScene) {
            winScene->winstage = changeid;
        }
        Engine::GameEngine::GetInstance().ChangeScene("win");
    }
}
