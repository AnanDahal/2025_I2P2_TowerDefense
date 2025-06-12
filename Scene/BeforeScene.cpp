//
// Created by Farrel on 6/12/2025.
//

#include "BeforeScene.h"
#include "PlayScene.hpp"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/IControl.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"

#include <iostream>
#include <allegro5/allegro_primitives.h>

#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Engine/GameEngine.hpp"

void BeforeScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    
    // Reset variables
    chatBox = nullptr;
    currentDialogueIndex = 0;
    isCleanedUp = false;
    isFading = false;
    fadeAlpha = 0.0f;
    fadeSpeed = 5.0f;
    dialogues.clear();
    
    // Make sure any previous character image is properly removed
    if (characterImage) {
        RemoveObject(characterImage->GetObjectIterator());
        characterImage = nullptr;
    }

    // Add dialogues based on story ID
    if (storyid == 1) {
        AddDialogue("Player", "Hello there.");
        AddDialogue("Enemy", "You shall not pass.");
        AddDialogue("Player", "Watch me.");
        
        // Set character image for story 1
        SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
    }
    else if (storyid == 3) {
        AddDialogue("Panda", "This is story 3.");
        AddDialogue("Enemy", "Really? Nice implementation");
        AddDialogue("Dev", "Thanks bro.");
        
        // Set character image for story 3
        SetCharacterImage("play/benjamin.png", 350, 450, halfW, halfH);
    }
    else {
        // Default dialogues for any other story ID
        AddDialogue("Player", "Let's start the game.");
        AddDialogue("System", "Stage " + std::to_string(storyid) + " loading...");
        
        // Default character image
        SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
    }

    // Create a new ChatBox (after character image so it's drawn on top)
    chatBox = new Engine::ChatBox(w, h, "", "");
    AddNewControlObject(chatBox);

    // Start the dialogue system
    StartDialogues();

    bgmInstance = AudioHelper::PlaySample("sass.wav", true, AudioHelper::BGMVolume);

    // Reset the cleanup flag
    isCleanedUp = false;
}

void BeforeScene::Terminate() {
    // Prevent double cleanup
    if (isCleanedUp) return;
    isCleanedUp = true;

    // Stop audio
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();

    // Explicitly remove character image before scene termination
    if (characterImage) {
        RemoveObject(characterImage->GetObjectIterator());
        characterImage = nullptr;
    }

    // Let IScene handle the cleanup of all objects (including chatBox)
    IScene::Terminate();
}

void BeforeScene::Draw() const {
    // Draw background first
    al_clear_to_color(al_map_rgb(0, 0, 100));
    
    // Draw all objects (including character image)
    IScene::Draw();
    
    // Draw fade effect on top of everything
    if (isFading) {
        int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
        al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, fadeAlpha));
    }
}

void BeforeScene::Update(float deltaTime) {
    IScene::Update(deltaTime);

    // Check if the current dialogue is finished and the user pressed a key
    if (chatBox && chatBox->finished) {
        NextDialogue();
        chatBox->finished = false;
    }

    // Update fade effect
    if (isFading) {
        fadeAlpha += fadeSpeed;
        if (fadeAlpha >= 255) {
            fadeAlpha = 255;
            // Change scene when fully faded out
            auto *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
            if (scene) {  // Add null check
                scene->MapId = storyid;
                Engine::GameEngine::GetInstance().ChangeScene("play");
            }
        }
    }
}

void BeforeScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);

    // Handle Enter key to change scene when dialogues are finished
    if (keyCode == ALLEGRO_KEY_ENTER) {
        if (currentDialogueIndex >= dialogues.size()) {
            // All dialogues completed, start fade out
            StartFadeOut();
            return;
        }
    }

    // Pass key events to the chat box if it exists
    if (chatBox) {
        chatBox->OnKeyDown(keyCode);
    }
}

void BeforeScene::BackOnClick(int stage) {
    // Prevent double cleanup
    if (isCleanedUp) return;

    // Start fade out instead of immediately changing scene
    StartFadeOut();
}

void BeforeScene::AddDialogue(const std::string& speaker, const std::string& text) {
    dialogues.push_back({speaker, text});
}

void BeforeScene::StartDialogues() {
    if (!dialogues.empty() && chatBox) {
        currentDialogueIndex = 0;
        chatBox->SetSpeaker(dialogues[0].speaker);
        chatBox->SetText(dialogues[0].text);
    }
}

void BeforeScene::NextDialogue() {
    if (chatBox) {
        currentDialogueIndex++;
        if (currentDialogueIndex < dialogues.size()) {
            chatBox->SetSpeaker(dialogues[currentDialogueIndex].speaker);
            chatBox->SetText(dialogues[currentDialogueIndex].text);
            
            // Example: Change character image based on who's speaking
            int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
            int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
            int halfW = w / 2;
            int halfH = h / 2;
            
            if (storyid == 1) {
                if (dialogues[currentDialogueIndex].speaker == "Player") {
                    SetCharacterImage("play/benjamin.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Enemy") {
                    SetCharacterImage("play/turret-2.png", characterWidth, characterHeight, halfW, halfH);
                }
            } else if (storyid == 3) {
                if (dialogues[currentDialogueIndex].speaker == "Panda") {
                    SetCharacterImage("play/benjamin.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Enemy") {
                    SetCharacterImage("play/turret-2.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Dev") {
                    SetCharacterImage("play/shockwave.png", characterWidth, characterHeight, halfW, halfH);
                }
            }
        } else {
            // No more dialogues, start fade out
            StartFadeOut();
        }
    }
}

void BeforeScene::StartFadeOut() {
    if (!isFading) {
        isFading = true;
        fadeAlpha = 0.0f;
    }
}

void BeforeScene::SetCharacterImage(const std::string& imagePath, float width, float height, float x, float y) {
    // Remove existing character image if any
    if (characterImage) {
        RemoveObject(characterImage->GetObjectIterator());
        characterImage = nullptr;
    }
    
    // Store the parameters
    characterImagePath = imagePath;
    characterWidth = width;
    characterHeight = height;
    characterX = x;
    characterY = y;
    
    // Create and add the new character image
    // Position it slightly higher to be visible above the chat box
    characterImage = new Engine::Image(imagePath, x, y - 100, width, height, 0.5, 0.5);
    AddNewObject(characterImage);
}

void BeforeScene::UpdateCharacterPosition(float x, float y) {
    if (characterImage) {
        characterX = x;
        characterY = y;
        characterImage->Position.x = x;
        characterImage->Position.y = y;
    }
}

void BeforeScene::UpdateCharacterSize(float width, float height) {
    if (characterImage) {
        characterWidth = width;
        characterHeight = height;
        
        // We need to recreate the image with the new size
        SetCharacterImage(characterImagePath, width, height, characterX, characterY);
    }
}
