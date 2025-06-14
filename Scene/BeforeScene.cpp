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
        AddDialogue("Narrator", "Once upon a time, there was a panda named Hua and a fox named Tong Tong who shared a living space.");
        AddDialogue("Hua", "Yo dude, got the berries you asked for right here.", "happy");
        AddDialogue("Tong Tong", "Thanks bro! Love me some berries.", "happy");
        AddDialogue("Tong Tong", "By the way, are you feeling alright?", "default");
        AddDialogue("Hua", "What do you mean?", "default");
        AddDialogue("Tong Tong", "Idrk bro your injury yesterday looked pretty serious.", "sad");
        AddDialogue("Hua", "I just had a small accident and bumped my head a little.", "default");
        AddDialogue("Hua", "Should be chill bro, don't worry.", "happy");
        AddDialogue("Tong Tong", "Alright bro, time to sleep. Goodnight.", "happy");
        AddDialogue("Hua", "Alright Tong Tong, goodnight. Tong tong tong tong sahurrrr!", "happy");
        AddDialogue("Tong Tong", "Hua Hua Hua Hua! Goodnight bro stay safe.", "happy");

        // Set character image for story 1
        SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
    }
    else if (storyid == 2) {
        AddDialogue("Narrator", "After work...");
        AddDialogue("Hua", "Yo Tong. have you seen my phone? I forgot where I put it.", "sad");
        AddDialogue("Tong Tong", "Weren't you just doomscrolling bro?", "default");
        AddDialogue("Hua", "...Maybe?", "default");
        AddDialogue("Tong Tong", "Huh? What do you mean \"Maybe\"?", "sad");
        AddDialogue("Tong Tong", "Whatever bro, I think I saw your phone on the couch.", "default");
        AddDialogue("Hua", "Huh, well what do you know. Thanks Tong.", "happy");
        AddDialogue("Tong Tong", "Sure thing. You're probably tired, go to sleep dawg.", "happy");
        AddDialogue("Hua", "Alrighty bro, goodnight.", "happy");
        AddDialogue("Tong Tong", "Goodnight.", "default");
        AddDialogue("Tong Tong", "(I sure hope he's alright...)", "sad");
    }
    else if (storyid == 3) {
        AddDialogue("Narrator", "That night, Hua called Tong Tong to help get back home.");
        AddDialogue("Tong Tong", "You knew that place didn't have data, yet you still chose to go here.", "sad"); //tong sad
        AddDialogue("Tong Tong", "This was all partially your fault bro.", "neutral"); //tong neutral
        AddDialogue("Hua", "My bad bro, could've sworn that place had data.", "happy"); //hua happy
        AddDialogue("Hua", "Besides, back then...", "happy"); //hua happy
        AddDialogue("Hua", "Umm...I'm sure I had a good reason.", "neutral"); //hua neutral
        AddDialogue("Hua", "Also, where is this? I thought we were going home?", "neutral"); //hua neutral
        AddDialogue("Tong Tong", "Alright that's it. I found it mildly amusing but this is pushing it bro.", "angry"); //tong angry
        AddDialogue("Tong Tong", "Somethings up with you for sure, might need to take you to the hospital.", "sad"); //tong sad
        AddDialogue("Hua", "*yawns* Maybe not right now? I'm feeling rather sleepy.", "neutral"); //hua neutral
        AddDialogue("Tong Tong", "...Fine. I guess it is pretty late already.", "neutral"); //tong neutral
        AddDialogue("Hua", "Exactly bro, now lets go home!", "happy"); //hua happy
        AddDialogue("Tong Tong", "...", "neutral"); //tong neutral
        AddDialogue("Tong Tong", "(I really hope this dude don't got no alzheimer's.)", "sad"); //tong sad

        // Set character image for story 3
        SetCharacterImage("play/benjamin.png", 350, 450, halfW, halfH);
    }
    else if (storyid == 4) {
        AddDialogue("Narrator", "Hua has been a bit stubborn the entire day.");
        AddDialogue("Hua", "Please bro, just let me rest...I'm not feeling so good.", "headache"); //hua headache
        AddDialogue("Tong Tong", "I've been trying to get you out of that bed for hours now.", "sad"); // tong sad
        AddDialogue("Tong Tong", "This is for your own good bro please...", "sad"); // tong sad
        AddDialogue("Hua", "Grrr...Just SHUT UP ALREADY!", "angry"); //hua angry
        AddDialogue("Hua", "I'll feel better after I rest, which is what you're preventing me from doing.", "angry"); //hua angry
        AddDialogue("Hua", "Wish I never had such an annoying roommate like you.", "headache"); //hua headache
        AddDialogue("Tong Tong", "...Fine.", "neutral"); // tong neutral
        AddDialogue("Tong Tong", "Fine! I was only trying to help.", "angry"); // tong angry
        AddDialogue("Tong Tong", "After all the things I've done for you, you still treat me like this.", "angry"); // tong angry
        AddDialogue("Tong Tong", "Go take care of yourself from now on.", "angry"); // tong angry
        AddDialogue("Hua", "Finally.", "neutral"); //hua neutral
    }
    else if (storyid == 5) {
        AddDialogue("Narrator", "At the hospital...");
        AddDialogue("|", "|");
        AddDialogue("| l", "| l");
        AddDialogue("I l", "I l");
        AddDialogue("Doctor", "Your friend here is showing symptoms of late-stage alzheimer's.", "doctor"); //doctor
        AddDialogue("Doctor", "At this point, he may have already lost a lot of his memories.", "doctor"); //doctor
        AddDialogue("| _", "| _");
        AddDialogue("Tong Tong", "*faces Hua* Yo Hua, how are you feeling?", "concerned"); //tong concerned
        AddDialogue("Hua", "...Mmmm...hello..?", "hospbed"); //hua hospbed
        AddDialogue("Tong Tong", "Hey dude, came as soon as I got the call.", "sad"); //tong sad
        AddDialogue("Hua", "...", "hospbed"); //hua hospbed
        AddDialogue("Hua", "Who are you again?", "hospbed"); //hua hospbed
        AddDialogue("Tong Tong", "...", "sad"); //tong sad
        AddDialogue("Doctor", "Its time for his rest. You can wait outside if you wish to stay.", "doctor"); //doctor
        AddDialogue("Tong Tong", "...Okay", "sad"); //tong sad
        SetCharacterImage("play/benjamin.png", 300, 400, halfW, halfH);
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

void BeforeScene::AddDialogue(const std::string& speaker, const std::string& text, const std::string& mood) {
    DialogueLine line = {speaker, text, mood};
    dialogues.push_back(line);
}

void BeforeScene::StartDialogues() {
    if (!dialogues.empty() && chatBox) {
        currentDialogueIndex = 0;
        chatBox->SetSpeaker(dialogues[0].speaker);
        chatBox->SetText(dialogues[0].text);
    }
}

void BeforeScene::NextDialogue() {
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
    // Position it higher to avoid overlap with chat box (y - 150 instead of y - 100)
    characterImage = new Engine::Image(imagePath, x, y - 150, width, height, 0.5, 0.5);
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

void BeforeScene::SetCharacterImageByMood(const std::string& character, const std::string& mood) {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    
    std::string imagePath;
    
    // Determine image path based on character and mood - simplified to 5 moods
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
        // Print debug info to console
        std::cout << "Setting Tong Tong mood: " << mood << std::endl;
        
        if (mood == "default") {
            imagePath = "play/scene/foxNeutral.png";
        } else if (mood == "happy") {
            // Try alternative paths if the original doesn't work
            imagePath = "play/scene/foxHappy.png";
            std::cout << "Using path: " << imagePath << std::endl;
        } else if (mood == "sad") {
            imagePath = "play/scene/foxSad.png";
        } else if (mood == "mixed") {
            imagePath = "play/scene/foxSadHappy.png";
        } else if (mood == "angry") {
            imagePath = "play/scene/foxMad.png";
        } else {
            // Default to normal expression
            imagePath = "play/scene/foxNeutral.png";
        }
    } else if (character == "Doctor") {
        imagePath = "play/scene/doctor.png";
    }
    // AddDialogue("|", "|");
    // AddDialogue("| l", "| l");
    // AddDialogue("I l", "I l");
    // AddDialogue("| _", "| _");
    else if (character == "|") {
        imagePath = "play/scene/loss1.png";
    } else if (character == "| l") {
        imagePath = "play/scene/loss2.png";
    } else if (character == "I l") {
        imagePath = "play/scene/loss3.png";
    } else if (character == "| _") {
        imagePath = "play/scene/loss4.png";
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
    
    // Print the final path being used
    std::cout << "Final image path: " << imagePath << std::endl;
    
    // Set the character image
    SetCharacterImage(imagePath, characterWidth, characterHeight, halfW, halfH);
}
