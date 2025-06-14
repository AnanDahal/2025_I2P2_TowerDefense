//
// Created by Farrel on 6/13/2025.
//

#ifndef AFTERSCENE_H
#define AFTERSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>
#include <vector>

#include "Engine/IScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ChatBox.hpp"

// Structure to hold dialogue lines
struct AfterDialogueLine {
    std::string speaker;
    std::string text;
    std::string mood;  // Added mood field
};

class AfterScene final : public Engine::IScene {
private:
    Engine::ChatBox* chatBox = nullptr;
    std::vector<AfterDialogueLine> dialogues;
    size_t currentDialogueIndex = 0;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    bool isCleanedUp = false;

    // Character image
    Engine::Image* characterImage = nullptr;
    std::string characterImagePath;
    float characterWidth = 300;
    float characterHeight = 400;
    float characterX = 0;
    float characterY = 0;

    // Fade system
    bool isFadingIn = false;
    bool isFadingOut = false;
    float fadeAlpha = 255.0f;  // Start fully faded in
    float fadeSpeed = 5.0f;    // Alpha change per frame

public:
    int storyid;  // Default story ID

    explicit AfterScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keyCode) override;
    void BackOnClick(int stage);

    // Methods for dialogue system
    void AddDialogue(const std::string& speaker, const std::string& text, const std::string& mood = "");
    void StartDialogues();
    void NextDialogue();
    void ClearDialogues();

    // Character image methods
    void SetCharacterImage(const std::string& imagePath, float width, float height, float x, float y);
    void UpdateCharacterPosition(float x, float y);
    void UpdateCharacterSize(float width, float height);
    void SetCharacterImageByMood(const std::string& character, const std::string& mood);

    // Fade methods
    void StartFadeIn();
    void StartFadeOut();
};

#endif //AFTERSCENE_H
