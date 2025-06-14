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
            AddDialogue("Hua", "Ughhhh...", "headache");
            AddDialogue("Hua", "What the heck was that dream?", "headache");
            AddDialogue("Hua", "Why'd it feel so...", "neutral");
            AddDialogue("Hua", "...real?", "neutral");
            SetCharacterImage("play/scene/pandaHeadache.png", 400, 500, halfW, halfH + 50);
            break;

        case 2:
            AddDialogue("Narrator", "Hua woke up with a headache once more.");
            AddDialogue("Hua", "Ouch...my head...", "headache");
            AddDialogue("Tong Tong", "Dude, what are you still doing here? Aren't you supposed to be at work?", "default");
            AddDialogue("Hua", "Eh, thought it was still 7 so I slept some more. 5 minutes right?", "default");
            AddDialogue("Tong Tong", "Bro, its almost 9.", "default");
            AddDialogue("Hua", "OH SHOOT! Time to go!", "happy");
            AddDialogue("Tong Tong", "...", "default");
            AddDialogue("Hua", "Umm...Tong, where's the bathroom again?", "sad");
            AddDialogue("Tong Tong", "?", "default");
            AddDialogue("Tong Tong", "Is this guy really joking right now?", "sad");
            SetCharacterImage("play/scene/pandaHeadache.png", 400, 500, halfW, halfH + 50);
            break;

        case 3:
            AddDialogue("Narrator", "And again...");
            AddDialogue("Hua", "Ughhhhhh...", "headache");
            AddDialogue("Tong Tong", "Are you alright bud? You're not looking too fresh.", "concerned");
            AddDialogue("Hua", "I don't know bro, I think I'm just gonna go back to sleep.", "sad");
            AddDialogue("Tong Tong", "No really, you look absolutely terrible.", "sad");
            AddDialogue("Tong Tong", "I think we really need to get you to the hospital.", "sad");
            AddDialogue("Hua", "...No thanks.", "default");
            AddDialogue("Tong Tong", "Huff, this guy...", "sad");
            SetCharacterImage("play/scene/pandaHeadache.png", 400, 500, halfW, halfH + 50);
            break;

        case 4:
            AddDialogue("Hua", "Ughhhhhhh...!", "headache");
            AddDialogue("Narrator", "For context, since none of the developers have the time / skill to draw this in a different fashion,");
            AddDialogue("Narrator", "Hua looks and feels like he's actively dying faster.");
            AddDialogue("Hua", "Ugh...please...let this...work...", "sad");
            AddDialogue("Hua", "*beep beep beep*", "sad");
            AddDialogue("911 Operator", "911, what's your emergency?", "default");
            AddDialogue("Hua", "Help...", "sad");
            SetCharacterImage("play/scene/pandaSick.png", 400, 500, halfW, halfH + 50);
            break;

        case 5:
            // add the loss scene
            AddDialogue("Doctor", "Mr Tong.", "default");
            AddDialogue("Tong Tong", "Yes doctor? Is he going to be alright?", "sad");
            AddDialogue("Doctor", "I'm afraid most of his brain functions are beginning to shutdown", "default");
            AddDialogue("Doctor", "He is currently only able to move his mouth.", "default");
            AddDialogue("Tong Tong", "...", "sad");
            AddDialogue("Doctor", "We don't know how long he has left, but I assume its not long left.", "default");
            AddDialogue("Tong Tong", "Can I please see him now doctor?", "sad");
            AddDialogue("Doctor", "By all means.", "default");
            SetCharacterImage("play/scene/doctor.png", 400, 500, halfW, halfH + 50);
            break;

        case 6:
            AddDialogue("Hua", "You know, you remind me of an old friend...", "hospbed");
            AddDialogue("Hua", "...someone really dear to me.", "hospbed");
            AddDialogue("Tong Tong", "...", "sad");
            AddDialogue("Hua", "Why so serious?", "hospbed");
            AddDialogue("Hua", "Theres no need to be sad.", "hospbed");
            AddDialogue("Tong Tong", "...Yeah.", "mixed");
            SetCharacterImage("play/scene/pandaSick.png", 400, 500, halfW, halfH + 50);
            break;

        default:
            // Default dialogues for any other story ID
            AddDialogue("Player", "We've won the battle!", "happy");
            AddDialogue("System", "Stage " + std::to_string(storyid) + " completed!");
            SetCharacterImage("play/scene/pandaHappy.png", 400, 500, halfW, halfH + 50);
            break;
    }

    // Create a new ChatBox (after character image so it's drawn on top)
    chatBox = new Engine::ChatBox(w, h, "", "");
    AddNewControlObject(chatBox);

    // Start the dialogue system
    StartDialogues();

    // Play victory music
    bgmInstance = AudioHelper::PlaySample("yui.wav", true, AudioHelper::BGMVolume);

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

void AfterScene::AddDialogue(const std::string& speaker, const std::string& text, const std::string& mood) {
    AfterDialogueLine line = {speaker, text, mood};
    dialogues.push_back(line);
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
        std::string speaker = dialogues[currentDialogueIndex].speaker;
        std::string text = dialogues[currentDialogueIndex].text;
        std::string mood = dialogues[currentDialogueIndex].mood;
        
        chatBox->SetSpeaker(speaker);
        chatBox->SetText(text);
        
        // If no explicit mood is provided, try to infer it from text content
        if (mood.empty()) {
            if (text.find("happy") != std::string::npos || 
                text.find("berries") != std::string::npos || 
                text.find("Thanks") != std::string::npos ||
                text.find("Love") != std::string::npos ||
                text.find("chill") != std::string::npos ||
                text.find("goodnight") != std::string::npos) {
                mood = "happy";
            }
            else if (text.find("sad") != std::string::npos || 
                    text.find("...") != std::string::npos ||
                    text.find("hope") != std::string::npos ||
                    text.find("alzheimer") != std::string::npos) {
                mood = "sad";
            }
            else if (text.find("confused") != std::string::npos || 
                    text.find("Ugh") != std::string::npos || 
                    text.find("Maybe") != std::string::npos ||
                    text.find("phone") != std::string::npos ||
                    text.find("forgot") != std::string::npos) {
                mood = "headache";
            }
            else if (text.find("angry") != std::string::npos || 
                    text.find("Whatever") != std::string::npos ||
                    text.find("SHUT UP") != std::string::npos ||
                    text.find("annoying") != std::string::npos ||
                    text.find("Grrr") != std::string::npos) {
                mood = "angry";
            }
            else {
                mood = "default";
            }
        }
        
        // Set character image based on speaker and mood
        SetCharacterImageByMood(speaker, mood);
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
    // Store the parameters
    characterImagePath = imagePath;
    characterWidth = width;
    characterHeight = height;
    characterX = x;
    characterY = y;
    
    // Remove existing character image if any
    if (characterImage) {
        RemoveObject(characterImage->GetObjectIterator());
        characterImage = nullptr;
    }
    
    // Create and add the new character image
    // Position it higher to avoid overlap with chat box (y - 200 instead of y - 150)
    characterImage = new Engine::Image(imagePath, x, y - 200, width, height, 0.5, 0.5);
    AddNewObject(characterImage);
    
    // Don't modify the chatBox handling - keep it as it was
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

void AfterScene::SetCharacterImageByMood(const std::string& character, const std::string& mood) {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    
    std::string imagePath;
    float characterWidth = 400;  // Increased from 300
    float characterHeight = 500; // Increased from 400
    
    // Determine image path based on character and mood
    if (character == "Hua" || character == "Player") {
        if (mood == "default") {
            imagePath = "play/scene/pandaNeutral.png";
        } else if (mood == "happy") {
            imagePath = "play/scene/pandaHappy.png";
        } else if (mood == "sad") {
            imagePath = "play/scene/pandaSad.png";
        } else if (mood == "headache") {
            imagePath = "play/scene/pandaHeadache.png";
        } else if (mood == "hospbed") {
            imagePath = "play/scene/pandaSick.png";
        } else if (mood == "angry") {
            imagePath = "play/scene/pandaAngry.png";
        } else {
            // Default to normal expression
            imagePath = "play/scene/pandaNeutral.png";
        }
    } else if (character == "Tong Tong") {
        if (mood == "default") {
            imagePath = "play/scene/foxNeutral.png";
        } else if (mood == "happy") {
            imagePath = "play/scene/foxHappy.png";
        } else if (mood == "sad") {
            imagePath = "play/scene/foxSad.png";
        } else if (mood == "mixed") {
            imagePath = "play/scene/foxSadHappy.png";
        } else if (mood == "angry") {
            imagePath = "play/scene/foxAngry.png";
        } else {
            // Default to normal expression
            imagePath = "play/scene/foxNeutral.png";
        }
    } else if (character == "Doctor") {
        imagePath = "play/scene/doctor.png";
    } else if (character == "911 Operator") {
        imagePath = "play/scene/operator.png";
    } else if (character == "Narrator") {
        // No image for narrator
        if (characterImage) {
            RemoveObject(characterImage->GetObjectIterator());
            characterImage = nullptr;
        }
        return;
    } else {
        // Default to no image for other speakers
        if (characterImage) {
            RemoveObject(characterImage->GetObjectIterator());
            characterImage = nullptr;
        }
        return;
    }
    
    // Set the character image with larger dimensions
    // Also position it a bit lower (y + 50)
    SetCharacterImage(imagePath, characterWidth, characterHeight, halfW, halfH + 50);
}
