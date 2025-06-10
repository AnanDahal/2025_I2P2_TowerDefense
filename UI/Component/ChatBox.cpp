#include "ChatBox.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>

namespace Engine {
    ChatBox::ChatBox(float screenW, float screenH, const std::string& chatText)
        : Image("stage-select/chat_box.png", (screenW-1052)/2, (screenH-237)-20, 1052, 237, 0, 0),
          chatText(chatText) {
        currentDisplayText = "";
        textTimer = 0;
        textSpeed = 10.0f;  // characters per second
        currentCharIndex = 0;
        std::cout << "[ChatBox] Constructed with chatText: '" << chatText << "'\n";
    }

    void ChatBox::Update(float deltaTime) {
        textTimer += deltaTime;
        if (textTimer >= 1.0f / textSpeed && currentCharIndex < chatText.length()) {
            currentDisplayText += chatText[currentCharIndex];
            currentCharIndex++;
            textTimer -= 1.0f / textSpeed;
            // Clamp timer so it doesn't add more than one char per update
            if (textTimer > 1.0f / textSpeed)
                textTimer = 0;
        }
    }

    void ChatBox::Draw() const {
        // std::cout << "[ChatBox] Draw called. currentDisplayText: '" << currentDisplayText << "'\n";
        Image::Draw(); // Draw the chat box background

        ALLEGRO_FONT* font = al_load_ttf_font("Resource/fonts/pirulen.ttf", 22, 0);
        ALLEGRO_FONT* smallFont = al_load_ttf_font("Resource/fonts/pirulen.ttf", 14, 0);

        if (!font) std::cout << "[ChatBox] Failed to load font: pirulen.ttf (22)\n";
        if (!smallFont) std::cout << "[ChatBox] Failed to load font: pirulen.ttf (14)\n";

        if (font && smallFont) {
            // Lower the main chat text
            float textX = Position.x + 30;
            float textY = Position.y + 70; // Increased from +30 to +70
            float textW = Size.x - 60;
            al_draw_multiline_text(font, al_map_rgb(0,0,0), textX, textY, textW, 40,
                                   ALLEGRO_ALIGN_LEFT, currentDisplayText.c_str());

            // Raise the "press ENTER" prompt
            if (currentCharIndex >= chatText.length()) {
                float promptX = Position.x + Size.x - 30;
                float promptY = Position.y + Size.y - 60; // Raised from -30 to -60
                al_draw_text(smallFont, al_map_rgb(128,128,128), promptX, promptY,
                             ALLEGRO_ALIGN_RIGHT, "press [ENTER] to continue");
            }

            al_destroy_font(font);
            al_destroy_font(smallFont);
        }
    }

    void ChatBox::OnKeyDown(int keycode) {
        std::cout << "[ChatBox] OnKeyDown called. keycode: " << keycode << "\n";
        // Check for spacebar (ALLEGRO_KEY_SPACE = 75)
        if (keycode == ALLEGRO_KEY_SPACE && currentCharIndex < chatText.length()) {
            currentDisplayText = chatText;
            currentCharIndex = chatText.length();
            std::cout << "[ChatBox] Space pressed, showing all text.\n";
        }

        if (keycode == ALLEGRO_KEY_ENTER && currentCharIndex >= chatText.length()) {
            finished = true;
        }
    }

    void ChatBox::Terminate() {
    }
}