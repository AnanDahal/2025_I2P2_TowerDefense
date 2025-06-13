#include "AfterScene.h"

#include "PlayScene.hpp"
#include "WinScene.hpp"
#include "StageSelectScene.hpp"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/IControl.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

void AfterScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    
    // Reset variables
    chatBox = nullptr;
    currentDialogueIndex = 0;
    isCleanedUp = false;
    isFadingIn = true;
    isFadingOut = false;
    fadeAlpha = 255.0f;  // Start fully faded in
    fadeSpeed = 5.0f;
    dialogues.clear();
    
    // Make sure any previous character image is properly removed
    if (characterImage) {
        RemoveObject(characterImage->GetObjectIterator());
        characterImage = nullptr;
    }

    // Add dialogues based on story ID
    switch (storyid) {
        case 1:
            AddDialogue("Hua", "Ughhhh...");
            AddDialogue("Hua", "What the heck was that dream?");
            AddDialogue("Hua", "Why'd it feel so...");
            AddDialogue("Hua", "...real?");
            SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
            break;
            
        case 2:
            AddDialogue("Narrator", "Hua woke up with a headache once more.");
            AddDialogue("Hua", "Ouch...my head...");
            AddDialogue("Tong Tong", "Dude, what are you still doing here? Aren't you supposed to be at work?");
            AddDialogue("Hua", "Eh, thought it was still 7 so I slept some more. 5 minutes right?");
            AddDialogue("Tong Tong", "Bro, its almost 9.");
            AddDialogue("Hua", "OH SHOOT! Time to go!");
            AddDialogue("Tong Tong", "...");
            AddDialogue("Hua", "Umm...Tong, where's the bathroom again?");
            AddDialogue("Tong Tong", "?");
            AddDialogue("Tong Tong", "Is this guy really joking right now?");
            SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
            break;
            
        case 3:
            AddDialogue("Narrator", "And again...");
            AddDialogue("Hua", "Ughhhhhh...");
            AddDialogue("Tong Tong", "Are you alright bud? You're not looking too fresh.");
            AddDialogue("Hua", "I don't know bro, I think I'm just gonna go back to sleep.");
            AddDialogue("Tong Tong", "No really, you look absolutely terrible.");
            AddDialogue("Tong Tong", "I think we really need to get you to the hospital.");
            AddDialogue("Hua", "...No thanks.");
            AddDialogue("Tong Tong", "Huff, this guy...");
            SetCharacterImage("play/panda.png", 350, 450, halfW, halfH);
            break;
            
        case 4:
            AddDialogue("Hua", "Ughhhhhhh...!");
            AddDialogue("Narrator", "For context, since none of the developers have the time / skill to draw this,");
            AddDialogue("Narrator", "Hua looks and feels like he's actively dying faster.");
            AddDialogue("Hua", "Ugh...please...let this...work...");
            AddDialogue("Hua", "*beep beep beep*");
            AddDialogue("911 Operator", "911, what's your emergency?");
            AddDialogue("Hua", "Help...");
            SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
            break;
            
        case 5:
            // add the loss scene
            AddDialogue("|", "|");
            AddDialogue("| l", "| l");
            AddDialogue("I l", "I l");
            AddDialogue("| _", "| _");
            AddDialogue("Doctor", "Mr Tong.");
            AddDialogue("Tong Tong", "Yes doctor? Is he going to be alright?");
            AddDialogue("Doctor", "Your friend here is showing symptoms of late-stage alzheimer's.");
            AddDialogue("Doctor", "I'm afraid most of his brain functions are beginning to shutdown");
            AddDialogue("Doctor", "He is currently only able to move his mouth.");
            AddDialogue("Tong Tong", "...");
            AddDialogue("Doctor", "We don't know how long he has left, but I assume its not long left.");
            AddDialogue("Tong Tong", "Can I please see him now doctor?");
            AddDialogue("Doctor", "By all means.");
            SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
            break;
            
        case 6:
            AddDialogue("Hua", "You know, you remind me of an old friend...");
            AddDialogue("Hua", "...someone really dear to me.");
            AddDialogue("Tong Tong", "...");
            AddDialogue("Hua", "Why so serious?");
            AddDialogue("Hua", "Theres no need to be sad.");
            AddDialogue("Tong Tong", "...Yeah.");
            SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
            break;
            
        default:
            // Default dialogues for any other story ID
            AddDialogue("Player", "We've won the battle!");
            AddDialogue("System", "Stage " + std::to_string(storyid) + " completed!");
            SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
            break;
    }

    // Create a new ChatBox (after character image so it's drawn on top)
    chatBox = new Engine::ChatBox(w, h, "", "");
    AddNewControlObject(chatBox);

    // Start the dialogue system
    StartDialogues();

    // Play victory music
    bgmInstance = AudioHelper::PlaySample("win.wav", true, AudioHelper::BGMVolume);

    // Reset the cleanup flag
    isCleanedUp = false;
}

void AfterScene::Terminate() {
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

void AfterScene::Draw() const {
    // Draw background first
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    // Draw all objects (including character image)
    IScene::Draw();
    
    // Draw fade effect on top of everything
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, fadeAlpha));
}

void AfterScene::Update(float deltaTime) {
    IScene::Update(deltaTime);

    // Handle fade in effect
    if (isFadingIn) {
        fadeAlpha -= fadeSpeed;
        if (fadeAlpha <= 0) {
            fadeAlpha = 0;
            isFadingIn = false;
        }
    }

    // Check if the current dialogue is finished and the user pressed a key
    if (chatBox && chatBox->finished) {
        NextDialogue();
        chatBox->finished = false;
    }

    // Handle fade out effect
    if (isFadingOut) {
        fadeAlpha += fadeSpeed;
        if (fadeAlpha >= 255) {
            fadeAlpha = 255;
            // Change scene when fully faded out - go back to campaign scene
            WinScene *scene = dynamic_cast<WinScene *>(Engine::GameEngine::GetInstance().GetScene("win"));
            scene->winstage = storyid;
            Engine::GameEngine::GetInstance().ChangeScene("win");
        }
    }
}

void AfterScene::OnKeyDown(int keyCode) {
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

void AfterScene::BackOnClick(int stage) {
    // Prevent double cleanup
    if (isCleanedUp) return;

    // Start fade out instead of immediately changing scene
    StartFadeOut();
}

void AfterScene::AddDialogue(const std::string& speaker, const std::string& text) {
    dialogues.push_back({speaker, text});
}

void AfterScene::StartDialogues() {
    if (!dialogues.empty() && chatBox) {
        currentDialogueIndex = 0;
        chatBox->SetSpeaker(dialogues[0].speaker);
        chatBox->SetText(dialogues[0].text);
    }
}

void AfterScene::NextDialogue() {
    if (!chatBox) return;
    
    currentDialogueIndex++;
    if (currentDialogueIndex < dialogues.size()) {
        chatBox->SetSpeaker(dialogues[currentDialogueIndex].speaker);
        chatBox->SetText(dialogues[currentDialogueIndex].text);
        
        // Change character image based on who's speaking
        int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
        int halfW = w / 2;
        int halfH = h / 2;
        
        // Character switching logic based on stage and speaker
        switch (storyid) {
            case 1:
            case 2:
            case 4:
            case 5:
                if (dialogues[currentDialogueIndex].speaker == "Player") {
                    SetCharacterImage("play/benjamin.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Ally") {
                    SetCharacterImage("play/turret-1.png", characterWidth, characterHeight, halfW, halfH);
                }
                break;
                
            case 3:
                if (dialogues[currentDialogueIndex].speaker == "Panda") {
                    SetCharacterImage("play/benjamin.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Ally") {
                    SetCharacterImage("play/turret-1.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Dev") {
                    SetCharacterImage("play/shockwave.png", characterWidth, characterHeight, halfW, halfH);
                }
                break;
                
            case 6:
                if (dialogues[currentDialogueIndex].speaker == "Player") {
                    SetCharacterImage("play/benjamin.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Ally") {
                    SetCharacterImage("play/turret-1.png", characterWidth, characterHeight, halfW, halfH);
                } else if (dialogues[currentDialogueIndex].speaker == "Dev") {
                    SetCharacterImage("play/shockwave.png", characterWidth, characterHeight, halfW, halfH);
                }
                break;
        }
    } else {
        // No more dialogues, start fade out
        StartFadeOut();
    }
}

void AfterScene::StartFadeIn() {
    if (!isFadingOut) {  // Don't start fade in if already fading out
        isFadingIn = true;
        isFadingOut = false;
        fadeAlpha = 255.0f;
    }
}

void AfterScene::StartFadeOut() {
    if (!isFadingIn) {  // Don't start fade out if still fading in
        isFadingOut = true;
        isFadingIn = false;
    }
}

void AfterScene::SetCharacterImage(const std::string& imagePath, float width, float height, float x, float y) {
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

void AfterScene::UpdateCharacterPosition(float x, float y) {
    if (characterImage) {
        characterX = x;
        characterY = y;
        characterImage->Position.x = x;
        characterImage->Position.y = y;
    }
}

void AfterScene::UpdateCharacterSize(float width, float height) {
    if (characterImage) {
        characterWidth = width;
        characterHeight = height;
        
        // We need to recreate the image with the new size
        SetCharacterImage(characterImagePath, width, height, characterX, characterY);
    }
}