//
// Created by Farrel on 6/12/2025.
//

#ifndef BEFORESCENE_H
#define BEFORESCENE_H
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro.h>
#include <vector>
#include <string>

#include "Engine/IScene.hpp"
#include "UI/Component/ChatBox.hpp"
#include "UI/Component/Image.hpp"

// Simple dialogue structure
struct DialogueLine {
    std::string speaker;
    std::string text;
};

class BeforeScene final : public Engine::IScene {
private:
    Engine::ChatBox* chatBox = nullptr;
    std::vector<DialogueLine> dialogues;
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
    bool isFading = false;
    float fadeAlpha = 0.0f;
    float fadeSpeed = 5.0f; // Alpha change per frame
    
public:
    explicit BeforeScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keyCode) override;
    void BackOnClick(int stage);
    
    // Methods for dialogue system
    void AddDialogue(const std::string& speaker, const std::string& text);
    void StartDialogues();
    void NextDialogue();
    
    // Character image methods
    void SetCharacterImage(const std::string& imagePath, float width, float height, float x, float y);
    void UpdateCharacterPosition(float x, float y);
    void UpdateCharacterSize(float width, float height);
    
    // Start the fade out effect
    void StartFadeOut();
    int storyid;
};

#endif //BEFORESCENE_H
